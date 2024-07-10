/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006        Annie.an        Create
** 
****************************************************************************/
#ifndef  _OD_GAME_ID_H_    
#define  _OD_GAME_ID_H_  
 /**-------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*****************************************************************************/
// 	Description : win,ctrl
/*****************************************************************************/
#define WIN_ID_DEF(win_id, win_id_name)          win_id,

typedef enum
{
OD_GAME_WIN_ID_START=MMI_MODULE_OD_GAME<< 16,
#include "od_game_id.def"
OD_GAME_MAX_WIN_ID
}OD_GAME_WINDOW_ID_E;

#undef WIN_ID_DEF

typedef enum
{
    OD_GAME_CTRL_ID_START = OD_GAME_MAX_WIN_ID, 
    OD_GAME_MENU_CTRL_ID,
    OD_GAME_HELP_CTRL_ID,
    OD_GAME_SOUND_CTRL_ID,
    OD_GAME_HIGH_SCORES_CTRL_ID,
#ifdef OD_GAME_BILLING_SUPPORT
    OD_GAME_BUY_MENU_CTRL_ID,
    OD_GAME_TIPS_TEXT_CTRL_ID,
    OD_GAME_INPUT_CODE_LABEL_CTRL_ID,
    OD_GAME_INPUT_CODE_EDITBOX_CTRL_ID,
    OD_GAME_BUY_CONFIRM_SCREEN_CTRL_ID,
#endif
    OD_GAME_MAX_CTRL_ID
}OD_GAME_CTRL_ID_E;

#ifdef   __cplusplus
   }
#endif
#endif
