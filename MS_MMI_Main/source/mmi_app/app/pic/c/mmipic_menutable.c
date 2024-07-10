/****************************************************************************
** File Name:      mmipic_menutable.c                                       *
** Author:         vincent                                                  *
** Date:           2008.01.14                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:                                                             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 01/2008      vincent            Create
** 
****************************************************************************/
#ifdef WIN32
#include "std_header.h"
#endif

//macro replace
//#ifdef MMI_PIC_EDITOR_ENABLE
#ifdef PIC_EDITOR_SUPPORT
#include "guimenu.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_menutable.h"
#include "mmi_text.h"
#include "mmipic_text.h"
#include "mmipic_menutable.h"
#include "mmipic_image.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/

#ifndef PIC_EDIT_U200_SUPPORT
//main menu of setting
const GUIMENU_ITEM_T menu_pic_edit_func_opt[] =
{
    {ID_PIC_EDIT_SAVE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},STXT_SAVE, 0, 0, 0, NULL},
    {ID_PIC_EDIT_SAVEAS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_COMMON_SAVE_AS, 0, 0, 0, NULL},  
    {ID_PIC_EDIT_UNDO, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_PIC_UNDO, 0, 0, 0, NULL},
//       {ID_PIC_EDIT_ZOOM, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_PIC_ZOOM, 0, 0, 0, NULL},  
    {ID_PIC_EDIT_ROTATE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_ROTATE, 0, 0, 0, NULL},
    {ID_PIC_EDIT_INSERT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_ADD, 0, 0, 0, MENU_PIC_EDIT_INSERT_OPT},
    {ID_PIC_EDIT_ADJUST, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_ADJUST, 0, 0, 0, MENU_PIC_EDIT_ADJUST_OPT},
    {ID_PIC_EDIT_EFFECT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_EFFECT, 0, 0, 0, MENU_PIC_EDIT_EFFECT_OPT},
};

const GUIMENU_ITEM_T menu_pic_edit_insert_opt[] =
{
    {ID_PIC_EDIT_PASTE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_CLIP, 0, 0, 0, NULL},
    {ID_PIC_EDIT_CHARACTER, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIC_CHARACTER, 0, 0, 0, NULL},  
#ifdef PIC_EDIT_FRAME_SUPPORT 
    {ID_PIC_EDIT_FRAME, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_FRAME, 0, 0, 0, NULL},
#endif    
};


const GUIMENU_ITEM_T menu_pic_edit_adjust_opt[] =
{
    {ID_PIC_EDIT_SIZE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_SIZE, 0, 0, 0, NULL},
    {ID_PIC_EDIT_COLOR, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COLOR, 0, 0, 0, NULL},  
    {ID_PIC_EDIT_BRIGHTNESS, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_COMMON_BRIGHTNESS, 0, 0, 0, NULL},
    {ID_PIC_EDIT_CONSTRACT, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_COMM_CONTRAST, 0, 0, 0, NULL},
    {ID_PIC_EDIT_SATURATION, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_PIC_SATURATION, 0, 0, 0, NULL},  
    {ID_PIC_EDIT_HUE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_PIC_HUE, 0, 0, 0, NULL},
};


const GUIMENU_ITEM_T menu_pic_edit_effect_opt[] =
{
    {ID_PIC_EDIT_GRAY, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_GRAY, 0, 0, 0, NULL},
    {ID_PIC_EDIT_SEPIA, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_SEPIA, 0, 0, 0, NULL},     
    {ID_PIC_EDIT_SHARPEN, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_SHARPEN, 0, 0, 0, NULL},
    {ID_PIC_EDIT_SMOOTH, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIC_SMOOTH, 0, 0, 0, NULL},  
    {ID_PIC_EDIT_DIFFUSION, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIC_DIFFUSION, 0, 0, 0, NULL}, 
    {ID_PIC_EDIT_EMBOSS, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_EMBOSS, 0, 0, 0, NULL},
    {ID_PIC_EDIT_EXPOSURE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_EXPOSURE, 0, 0, 0, NULL},
    {ID_PIC_EDIT_NEGATIVE_RGB, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_NEGATIVE_RGB, 0, 0, 0, NULL},
    {ID_PIC_EDIT_DENOISE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_DENOISE, 0, 0, 0, NULL},
};

#else //PDA style menu

//main menu of setting
const GUIMENU_ITEM_T menu_pic_edit_func_opt[] =
{
    {ID_PIC_EDIT_BRIGHTNESS, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_BRIGHT_CONSTRACT, IMAGE_PIC_BRIGHTNESS_CONSTRACT, 0, 0, NULL},
    {ID_PIC_EDIT_HUE,        TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_HUE_SATURATION, IMAGE_PIC_HUE_SATURATION, 0, 0, NULL},
    {ID_PIC_EDIT_EFFECT,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_EFFECT, IMAGE_PIC_EFFECT, 0, 0, MENU_PIC_EDIT_EFFECT_OPT},
    {ID_PIC_EDIT_SIZE,       TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_PDA_ADJUST_SIZE, IMAGE_PIC_ADJUST_SIZE, 0, 0, NULL},
    {ID_PIC_EDIT_ROTATE,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_ROTATE, IMAGE_PIC_ROTATE, 0, 0, NULL},
    {ID_PIC_EDIT_INSERT,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_ADD, 0, 0, 0, MENU_PIC_EDIT_INSERT_OPT},
    {ID_PIC_EDIT_COLOR,      TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_PDA_ADJUST_COLOR, 0, 0, 0, NULL},
};

const GUIMENU_ITEM_T menu_pic_edit_insert_opt[] =
{
    {ID_PIC_EDIT_PASTE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_CLIP, 0, 0, 0, NULL},
    {ID_PIC_EDIT_CHARACTER, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIC_CHARACTER, 0, 0, 0, NULL},  
#ifdef PIC_EDIT_FRAME_SUPPORT 
    {ID_PIC_EDIT_FRAME, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_FRAME, 0, 0, 0, NULL},
#endif    
};


const GUIMENU_ITEM_T menu_pic_edit_adjust_opt[] =
{
    {ID_PIC_EDIT_SIZE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_SIZE, 0, 0, 0, NULL},
    {ID_PIC_EDIT_COLOR, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_COLOR, 0, 0, 0, NULL},  
    {ID_PIC_EDIT_BRIGHTNESS, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_COMMON_BRIGHTNESS, 0, 0, 0, NULL},
    {ID_PIC_EDIT_CONSTRACT, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_COMM_CONTRAST, 0, 0, 0, NULL},
    {ID_PIC_EDIT_SATURATION, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_PIC_SATURATION, 0, 0, 0, NULL},  
    {ID_PIC_EDIT_HUE, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_PIC_HUE, 0, 0, 0, NULL},
};


const GUIMENU_ITEM_T menu_pic_edit_effect_opt[] =
{
    {ID_PIC_EDIT_GRAY, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_GRAY, 0, 0, 0, NULL},
    {ID_PIC_EDIT_SEPIA, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_SEPIA, 0, 0, 0, NULL},     
    {ID_PIC_EDIT_SHARPEN, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_SHARPEN, 0, 0, 0, NULL},
    {ID_PIC_EDIT_SMOOTH, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIC_SMOOTH, 0, 0, 0, NULL},  
    {ID_PIC_EDIT_DIFFUSION, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN}, TXT_PIC_DIFFUSION, 0, 0, 0, NULL}, 
    {ID_PIC_EDIT_EMBOSS, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_EMBOSS, 0, 0, 0, NULL},
    {ID_PIC_EDIT_EXPOSURE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_EXPOSURE, 0, 0, 0, NULL},
    {ID_PIC_EDIT_NEGATIVE_RGB, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_NEGATIVE_RGB, 0, 0, 0, NULL},
    {ID_PIC_EDIT_DENOISE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_PIC_DENOISE, 0, 0, 0, NULL},
};

#endif //PIC_EDIT_U200_SUPPORT


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmipic_menu_table[] = 
{
    {NULL},
    #include "mmipic_menutable.def"
};

#undef MENU_DEF

/*****************************************************************************/
//  Description : Register pic menu group
//  Global resource dependence : none
//  Author: vincent.shang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPIC_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_PIC, mmipic_menu_table);
}

#ifdef   __cplusplus
    }
#endif
#endif
