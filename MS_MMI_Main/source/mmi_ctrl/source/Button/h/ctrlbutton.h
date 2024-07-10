/***************************************************************************
** File Name:      ctrlbutton.h                                            *
** Author:         hua.fang                                                *
** Date:           08/03/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _CTRLBUTTON_H_
#define _CTRLBUTTON_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "guistring.h"
#include "mmitheme_button.h"

#include "ctrlbase.h"
#include "ctrlbutton_export.h"

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
// button type info
#define CTRL_BUTTON_TYPE     (BUTTON_GetType())

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
//button fg and bg
typedef struct
{
    GUI_BG_T    bg;
    GUI_BG_T    fg;
} GUIBUTTON_BG_FG_T;

// button init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    GUI_BG_T                    bg;
} CTRLBUTTON_INIT_PARAM_T;

// button struct
typedef struct _button_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    BOOLEAN                     is_gray;        //button is gray
    BOOLEAN                     is_transparent; //button image is transparent
    BOOLEAN                     is_permit_border;//button is permit display border
    BOOLEAN                     is_pressed;     //button is pressed
    BOOLEAN                     is_active;

    BOOLEAN                     is_send_long;   //is send long msg
    BOOLEAN                     is_handle_long; //is handle long msg
    BOOLEAN                     is_forbid_paint;//is forbid full paint update

    int16                       bg_display_x;   //background display x
    int16                       bg_display_y;   //background display y
    int16                       fg_display_x;   //background display x
    int16                       fg_display_y;   //background display y

    GUIBUTTON_STYLE_E           style;          // button style

    GUIBUTTON_BG_FG_T           release;        //release background and foreground
    GUIBUTTON_BG_FG_T           pressed;        //pressed background and foreground
    GUIBUTTON_BG_FG_T           gray;           //gray background and foreground

    GUISTR_TEXT_INFO_T          text_info;

    GUIBUTTON_CALLBACK_FUNC     callback_func;  //callback function
    GUIBUTTON_CALLBACK_FUNC_EXT callback_func_ext;  //callback function
    GUIBUTTON_OWNER_DRAW_FUNC   OwnerDrawFunc;  //owner draw function

    GUI_SHEEN_T                 *sheen_ptr;     //sheen pointer

    MMITHEME_BUTTON_T           theme;          //button theme
    
    MMI_HANDLE_T                win_handle;     //window handle

    GUI_LCD_DEV_INFO            pop_layer_info; //popblink按键特效自建层
} CTRLBUTTON_OBJ_T;

// button class for public or protected method
typedef struct _button_vtbl_tag
{
    CTRLBASE_VTBL_T             base_ctrl_vtbl;

    void    (*GetButtonBgOrFg)  (CTRLBUTTON_OBJ_T  *button_ctrl_ptr, GUI_BG_T *bg_ptr, BOOLEAN is_bg );
    uint16  (*GetButtonBgHeight)(CTRLBUTTON_OBJ_T  *button_ctrl_ptr);
    void    (*TpDown)           (CTRLBUTTON_OBJ_T  *button_ctrl_ptr);
    void    (*TpMoveOut)        (CTRLBUTTON_OBJ_T  *button_ctrl_ptr);
} CTRLBUTTON_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get button type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BUTTON_GetType();

/*****************************************************************************/
//  Description : get bg or fg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
static __inline void VTLBUTTON_GetBgOrFg(
                                      CTRLBUTTON_OBJ_T  *button_ctrl_ptr,   //in:
                                      GUI_BG_T          *bg_ptr,            //in/out:
                                      BOOLEAN           is_bg               //in:
                                      )
{
    CTRLBUTTON_VTBL_T  *button_ctrl_vtbl_ptr = (CTRLBUTTON_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BUTTON_TYPE, button_ctrl_ptr);
    button_ctrl_vtbl_ptr->GetButtonBgOrFg(button_ctrl_ptr, bg_ptr, is_bg);
}

/*****************************************************************************/
//  Description : tp down
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
static __inline void VTLBUTTON_TpDown(
                                   CTRLBUTTON_OBJ_T     *button_ctrl_ptr
                                   )
{
    CTRLBUTTON_VTBL_T  *button_ctrl_vtbl_ptr = (CTRLBUTTON_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BUTTON_TYPE, button_ctrl_ptr);

    if (PNULL != button_ctrl_vtbl_ptr->TpDown)
    {
        button_ctrl_vtbl_ptr->TpDown(button_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : tp move out
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
static __inline void VTLBUTTON_TpMoveOut(
                                      CTRLBUTTON_OBJ_T  *button_ctrl_ptr
                                      )
{
    CTRLBUTTON_VTBL_T  *button_ctrl_vtbl_ptr = (CTRLBUTTON_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BUTTON_TYPE, button_ctrl_ptr);

    if (PNULL != button_ctrl_vtbl_ptr->TpMoveOut)
    {
        button_ctrl_vtbl_ptr->TpMoveOut(button_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : get button bg height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
static __inline uint16 VTLBUTTON_GetButtonBgHeight(
                                                CTRLBUTTON_OBJ_T    *button_ctrl_ptr
                                                )
{
    uint16              height = 0;
    CTRLBUTTON_VTBL_T  *button_ctrl_vtbl_ptr = (CTRLBUTTON_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BUTTON_TYPE, button_ctrl_ptr);

    if (PNULL != button_ctrl_vtbl_ptr->GetButtonBgHeight)
    {
        height = button_ctrl_vtbl_ptr->GetButtonBgHeight(button_ctrl_ptr);
    }

    return height;
}

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _CTRLBUTTON_H_
