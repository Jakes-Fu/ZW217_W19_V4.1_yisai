#ifndef _OD_GAME_MENUTABLE_H
#define _OD_GAME_MENUTABLE_H

#include "mmi_module.h"
#include "guimenu.h"
#include "od_game_text.h"
#include "mmi_image.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                                                                       *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

   // const GUI_MENU_ITEM_T  menu_menu_quintet[];
/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                                                          */
/*-------------------------------------------------------------------------*/
typedef enum
{
    OD_GAME_ID_MENU_START = (MMI_MODULE_OD_GAME << 16)+100,	
    MENU_ID_OD_GAME_RESUME,
    MENU_ID_OD_GAME_NEW,
    MENU_ID_OD_GAME_NEW_2, 
    MENU_ID_OD_GAME_NEW_3, 
    MENU_ID_OD_GAME_HIGH_SCORES,
    MENU_ID_OD_GAME_SOUND,  
    MENU_ID_OD_GAME_HELP,   	
    OD_GAME_ID_MENU_MAX
}OD_GAME_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    OD_GAME_MENU_LABEL_START = (MMI_MODULE_OD_GAME << 16),
       #include "od_game_menutable.def" 
    OD_GAME_MENU_LABLE_MAX
} OD_GAME_MENU_LABEL_E;

#undef MENU_DEF


PUBLIC void od_game_reg_menu_group(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/
#endif /* MENUTABLE_H */
