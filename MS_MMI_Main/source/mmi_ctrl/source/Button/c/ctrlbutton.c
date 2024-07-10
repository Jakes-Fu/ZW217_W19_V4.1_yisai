/*****************************************************************************
** File Name:      ctrlbutton.c                                              *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2010      Jassmine              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_app.h"
#include "guicommon.h"
#include "guistring.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmk_tp.h"
#include "graphics_3d_rotate.h"//pop effect

#include "ctrlbutton.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*! @struct GUI_POP_INPUT_T
@brief button pop效果设置
*/
typedef struct
{
    int32            total_dis_frame;
    FIX16_POINT_T    centre_point;
    IMGREF_SIZE_T    show_size;
    void             *pop_buf_ptr;
} GUIBUTTON_POP_DRAW_INPUT_T;

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init button class
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonCtrlInitVtbl(
                              CTRLBUTTON_VTBL_T         *button_vtbl_ptr
                              );

/*****************************************************************************/
//  Description : button construct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonConstruct(
                               CTRLBASE_OBJ_T           *base_ctrl_ptr,
                               CTRLBUTTON_INIT_PARAM_T  *button_param_ptr
                               );

/*****************************************************************************/
//  Description : button destruct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonDestruct(
                              CTRLBASE_OBJ_T            *base_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : display button control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ButtonDraw(
                     CTRLBUTTON_OBJ_T           *button_ctrl_ptr,//in:
                     GUIBUTTON_OWNDRAW_STATE_E  draw_state
                     );

/*****************************************************************************/
//  Description : display button background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ButtonDrawBg(
                       CTRLBUTTON_OBJ_T         *button_ctrl_ptr//in:
                       );

/*****************************************************************************/
//  Description : display button foreground
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ButtonDrawFg(
                       CTRLBUTTON_OBJ_T         *button_ctrl_ptr//in:
                       );

/*****************************************************************************/
//  Description : display button text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ButtonDrawText(
                         CTRLBUTTON_OBJ_T       *button_ctrl_ptr
                         );

/*****************************************************************************/
//  Description : display pop effect
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ButtonDrawPopAnim(
                          CTRLBUTTON_OBJ_T      *button_ctrl_ptr,
                          int32                 total_dis_frame
                          );

/*****************************************************************************/
//  Description : Drawn PopBlinking Effect
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ButtonDrawPopBlinkingEffect(
                                 CTRLBUTTON_OBJ_T           *button_ctrl_ptr,
                                 GUIBUTTON_POP_DRAW_INPUT_T *pop_info_ptr
                                 );

#ifdef TOUCH_PANEL_SUPPORT 
/*****************************************************************************/
//  Description : handle button tp down message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ButtonHandleTpDown(
                                      CTRLBUTTON_OBJ_T      *button_ctrl_ptr,
                                      DPARAM                param
                                      );

/*****************************************************************************/
//  Description : handle button tp up message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ButtonHandleTpUp(
                                    CTRLBUTTON_OBJ_T        *button_ctrl_ptr,
                                    DPARAM                  param
                                    );

/*****************************************************************************/
//  Description : handle button tp long message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ButtonHandleTpLong(
                                      CTRLBUTTON_OBJ_T      *button_ctrl_ptr,
                                      DPARAM                param
                                      );

/*****************************************************************************/
//  Description : handle button tp move message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ButtonHandleTpMove(
                                      CTRLBUTTON_OBJ_T      *button_ctrl_ptr,
                                      DPARAM                param
                                      );
#endif

/*****************************************************************************/
//  Description : handle button msg function
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ButtonCtrlHandleMsg(
                                   CTRLBASE_OBJ_T           *base_ctrl_ptr,  //control pointer
                                   MMI_MESSAGE_ID_E         msg_id,     //message
                                   DPARAM                   param       //add data
                                   );

/*****************************************************************************/
//  Description : set button rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  ButtonCtrlSetRect(
                                CTRLBASE_OBJ_T          *base_ctrl_ptr,
                                const CAF_RECT_T        *rect_ptr
                                );

/*****************************************************************************/
//  Description : set button border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  ButtonCtrlSetBorder(
                                  CTRLBASE_OBJ_T        *base_ctrl_ptr,
                                  GUI_BORDER_T          *border_ptr
                                  );

/*****************************************************************************/
//  Description : set button font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  ButtonCtrlSetFont(
                                CTRLBASE_OBJ_T          *base_ctrl_ptr,
                                GUI_FONT_ALL_T          *font_ptr
                                );

/*****************************************************************************/
//  Description : get button font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  ButtonCtrlGetFont(
                                CTRLBASE_OBJ_T          *base_ctrl_ptr,
                                GUI_FONT_ALL_T          *font_ptr
                                );

/*****************************************************************************/
//  Description : get button height by width,include border,text etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonCtrlGetHeightByWidth(
                                         CTRLBASE_OBJ_T         *base_ctrl_ptr,  //in:
                                         uint16                 width,      //in:
                                         uint16                 *height_ptr //in/out:
                                         );

/*****************************************************************************/
//  Description : set button display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonCtrlSetDisplayRect(
                                       CTRLBASE_OBJ_T           *base_ctrl_ptr,  //in:
                                       const GUI_RECT_T         *rect_ptr,  //in:
                                       BOOLEAN                  is_update   //in:
                                       );

/*****************************************************************************/
//  Description : get button background or foreground
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ButtonCtrlGetBgOrFg(
                           CTRLBUTTON_OBJ_T     *button_ctrl_ptr,   //in:
                           GUI_BG_T             *bg_ptr,            //in/out:
                           BOOLEAN              is_bg               //in:
                           );

/*****************************************************************************/
//  Description : get button bg height
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 ButtonCtrlGetBgHeight(
                                CTRLBUTTON_OBJ_T    *button_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : get button rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetButtonRect(
                               CTRLBUTTON_OBJ_T     *button_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : calculate the foreground image and text region
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CalcImageTextRect(
                             CTRLBUTTON_OBJ_T       *button_ctrl_ptr,
                             GUI_RECT_T             *image_rect_ptr,        // [out] image dst rect
                             GUI_RECT_T             *text_rect_ptr          // [out] text dst rect
                             );

/*****************************************************************************/
//  Description : reset button sheen
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetButtonSheen(
                               CTRLBUTTON_OBJ_T             *button_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : get button pointer by control id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLBUTTON_OBJ_T* GetButtonPtr(
                                     MMI_HANDLE_T   ctrl_handle
                                     );

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ButtonTypeOf(
                           CTRLBASE_OBJ_T   *ctrl_ptr
                           );
    
/*****************************************************************************/
//  Description : get button bg and fg max height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetButtonBgFgHeight(
                                 GUI_BG_T               *bg_ptr,        //in:
                                 GUI_BG_T               *fg_ptr,        //in:
                                 CTRLBUTTON_OBJ_T       *button_ctrl_ptr//in:
                                 );

/*****************************************************************************/
//  Description : button destruct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonFreeText(
                             CTRLBUTTON_OBJ_T           *button_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : display button text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_STRING_T ButtonGetTextInfo(
                                     CTRLBUTTON_OBJ_T   *button_ctrl_ptr
                                     );

/****************************************************************************/    
//  Description: Create And Append Pop Layer
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalculatePopLayerRelativeRect(
                                               CTRLBUTTON_OBJ_T  *button_ctrl_ptr
                                               );

/****************************************************************************/    
//  Description: Create And Append Pop Layer
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateAndAppendPopLayer(
                                      CTRLBUTTON_OBJ_T      *button_ctrl_ptr
                                      );

/****************************************************************************/    
//  Description: Create Pop Blink Layer
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePopBlinkLayer(
                                  CTRLBUTTON_OBJ_T      *button_ctrl_ptr,
                                  GUI_RECT_T            *pop_dis_rect
                                  );

/****************************************************************************/    
//  Description :release pop layer
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ReleasePopBlinkLayer( 
                                CTRLBUTTON_OBJ_T        *button_ctrl_ptr
                                );

/****************************************************************************/    
//  Description :Set Default Pop Effect Radius
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void SetDefaultPopEffectRadius( 
                                     CTRLBUTTON_OBJ_T       *button_ctrl_ptr
                                     );

/*****************************************************************************/
//  Description : reset button pop
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetButtonPop(
                             CTRLBUTTON_OBJ_T       *button_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ButtonCtrlPackInitParam(
                                  GUIBUTTON_INIT_DATA_T     *button_init_ptr,   //in
                                  CTRLBUTTON_INIT_PARAM_T   *button_param_ptr   //out
                                  );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get button type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BUTTON_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register(
            CTRL_BASE_TYPE,
            "button",
            sizeof(CTRLBUTTON_OBJ_T),
            (OBJECT_CONSTRUCTOR_FUNC)ButtonConstruct,
            (OBJECT_DESTRUCTOR_FUNC)ButtonDestruct,
            sizeof(CTRLBUTTON_INIT_PARAM_T),
            (PARAM_PACKING_FUNC)ButtonCtrlPackInitParam,
            sizeof(CTRLBUTTON_VTBL_T),
            (VTBL_INIT_FUNC)ButtonCtrlInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init button vtbl
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonCtrlInitVtbl(
                              CTRLBUTTON_VTBL_T         *button_vtbl_ptr
                              )
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr =  (CTRLBASE_VTBL_T*)button_vtbl_ptr;

    base_ctrl_vtbl_ptr->HandleEvent = ButtonCtrlHandleMsg;
    base_ctrl_vtbl_ptr->SetRect =  ButtonCtrlSetRect;
    base_ctrl_vtbl_ptr->SetBorder =  ButtonCtrlSetBorder;
    base_ctrl_vtbl_ptr->SetFont =  ButtonCtrlSetFont;
    base_ctrl_vtbl_ptr->GetFont =  ButtonCtrlGetFont;
    base_ctrl_vtbl_ptr->GetHeightByWidth = ButtonCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->SetDisplayRect = ButtonCtrlSetDisplayRect;

    button_vtbl_ptr->GetButtonBgOrFg = ButtonCtrlGetBgOrFg;
    button_vtbl_ptr->GetButtonBgHeight = ButtonCtrlGetBgHeight;

    return TRUE;
}

/*****************************************************************************/
//  Description : button construct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonConstruct(
                               CTRLBASE_OBJ_T           *base_ctrl_ptr,
                               CTRLBUTTON_INIT_PARAM_T  *button_param_ptr
                               )
{
    BOOLEAN                 result = TRUE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = (CTRLBUTTON_OBJ_T*)base_ctrl_ptr;

    //set bg
    button_ctrl_ptr->release.bg = button_param_ptr->bg;

    //set permit display border
    button_ctrl_ptr->is_permit_border = TRUE;

    //set win handle
    button_ctrl_ptr->win_handle = MMK_GetWinHandleByCtrl(base_ctrl_ptr->handle);

    //get button theme
    MMITHEME_GetButtonTheme(&button_ctrl_ptr->theme);

    //set state
    VTLBASE_SetCanActive(base_ctrl_ptr,FALSE);

    return (result);
}

/*****************************************************************************/
//  Description : button destruct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonDestruct(
                              CTRLBASE_OBJ_T    *base_ctrl_ptr
                              )
{
    BOOLEAN                 result = TRUE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = (CTRLBUTTON_OBJ_T*)base_ctrl_ptr;

    // first, destruct itself
    if (PNULL != button_ctrl_ptr)
    {
        ButtonFreeText(button_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ButtonCtrlPackInitParam(
                                  GUIBUTTON_INIT_DATA_T     *button_init_ptr,   //in
                                  CTRLBUTTON_INIT_PARAM_T   *button_param_ptr   //out
                                  )
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*)button_param_ptr;

    base_ctrl_param_ptr->both_rect = button_init_ptr->both_rect;

    button_param_ptr->bg = button_init_ptr->bg;
}

/*****************************************************************************/
//  Description : handle button msg function
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ButtonCtrlHandleMsg(
                                    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //control pointer
                                    MMI_MESSAGE_ID_E    msg_id,     //message
                                    DPARAM              param       //add data
                                    )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = (CTRLBUTTON_OBJ_T*)base_ctrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != button_ctrl_ptr); /*assert verified*/
    if (PNULL == button_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_PAINT:
        if (!button_ctrl_ptr->is_forbid_paint)
        {
            ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_NONE);
        }
        break;

    case MSG_CTL_GET_ACTIVE:
        //notify parent control
        button_ctrl_ptr->is_active = TRUE;
        result = CTRLMSG_SendNotify(base_ctrl_ptr->handle,MSG_NOTIFY_GET_ACTIVE);

        // add for NEWMS00175248
        // delete for NEWMS00188102
        // NEWMS00175248 : PUBWIN的BUTTON不再获取焦点，所以不存在两个BUTTON都高亮的问题
        // NEWMS00188102 : 获取焦点后又隐藏，会有残影。其他控件active时没刷新，与其他控件保持一致
        //ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_NONE);
        break;

    case MSG_CTL_LOSE_ACTIVE:
        //notify parent control
        button_ctrl_ptr->is_active = FALSE;
        result = CTRLMSG_SendNotify(base_ctrl_ptr->handle,MSG_NOTIFY_LOSE_ACTIVE);
        //ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_NONE);
        break;
        
    case MSG_CTL_LOSE_FOCUS:
    case MSG_CTL_CLOSE:
    case MSG_LCD_SWITCH:
        //set release
        button_ctrl_ptr->is_pressed = FALSE;

        if ((GUIBUTTON_RUNSHEEN_ANIM == button_ctrl_ptr->theme.anim_type) &&
            (PNULL != button_ctrl_ptr->sheen_ptr))
        {
            //release sheen
            GUI_ReleaseSheen(button_ctrl_ptr->sheen_ptr,FALSE);
            button_ctrl_ptr->sheen_ptr = PNULL;
        }
        else if(GUIBUTTON_POPBLINK_ANIM == button_ctrl_ptr->theme.anim_type)
        {
            ReleasePopBlinkLayer(button_ctrl_ptr);
        }
        break;

    case MSG_APP_WEB:
        if (!button_ctrl_ptr->is_gray)
        {
            if (!button_ctrl_ptr->is_send_long)
            {
                if (PNULL != button_ctrl_ptr->callback_func)
                {
                    button_ctrl_ptr->callback_func();
                }
                else if (PNULL != button_ctrl_ptr->callback_func_ext)
                {
                    button_ctrl_ptr->callback_func_ext(base_ctrl_ptr->handle);
                }
                else
                {
                    //notify parent control
                    CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_MIDSK);
                }
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        //MMK_DelayTpMove();
        result = ButtonHandleTpDown(button_ctrl_ptr,param);
        break;

    case MSG_TP_PRESS_UP:
        result = ButtonHandleTpUp(button_ctrl_ptr,param);
        break;

    case MSG_TP_PRESS_LONG:
        result = ButtonHandleTpLong(button_ctrl_ptr,param);
        break;

    case MSG_TP_PRESS_MOVE:
        result = ButtonHandleTpMove(button_ctrl_ptr,param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  ButtonCtrlSetRect(
                                CTRLBASE_OBJ_T      *base_ctrl_ptr,
                                const CAF_RECT_T    *rect_ptr
                                )
{
    BOOLEAN                 result = TRUE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    button_ctrl_ptr = (CTRLBUTTON_OBJ_T*)base_ctrl_ptr;

    if (!GUI_EqualRect(base_ctrl_ptr->rect,*rect_ptr))
    {
        base_ctrl_ptr->rect         = *rect_ptr;
        base_ctrl_ptr->display_rect = *rect_ptr;
        
        GUI_SetVOrHRect( base_ctrl_ptr->handle, (GUI_RECT_T *)rect_ptr,&(base_ctrl_ptr->both_rect));
        
        //set release
        button_ctrl_ptr->is_pressed = FALSE;
        
        if ((GUIBUTTON_RUNSHEEN_ANIM == button_ctrl_ptr->theme.anim_type) &&
            (PNULL != button_ctrl_ptr->sheen_ptr))
        {
            //release sheen
            GUI_ReleaseSheen(button_ctrl_ptr->sheen_ptr,FALSE);
            button_ctrl_ptr->sheen_ptr = PNULL;
        }
        else if(GUIBUTTON_POPBLINK_ANIM == button_ctrl_ptr->theme.anim_type)
        {
            ReleasePopBlinkLayer(button_ctrl_ptr);
            SetDefaultPopEffectRadius(button_ctrl_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonCtrlSetBorder(
                                  CTRLBASE_OBJ_T    *base_ctrl_ptr,
                                  GUI_BORDER_T      *border_ptr
                                  )
{
    BOOLEAN                 result = TRUE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    button_ctrl_ptr = (CTRLBUTTON_OBJ_T*)base_ctrl_ptr;

    //set border
    button_ctrl_ptr->theme.border = *border_ptr;

    return (result);
}

/*****************************************************************************/
//  Description : set button font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonCtrlSetFont(
                                CTRLBASE_OBJ_T      *base_ctrl_ptr,
                                GUI_FONT_ALL_T      *font_ptr
                                )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    button_ctrl_ptr = (CTRLBUTTON_OBJ_T*)base_ctrl_ptr;
    if (PNULL != font_ptr)
    {
        //set font
        button_ctrl_ptr->theme.font = *font_ptr;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get button font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonCtrlGetFont(
                                CTRLBASE_OBJ_T      *base_ctrl_ptr,
                                GUI_FONT_ALL_T      *font_ptr
                                )
{
    BOOLEAN                 result = TRUE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    button_ctrl_ptr = (CTRLBUTTON_OBJ_T*)base_ctrl_ptr;

    //set font
    *font_ptr = button_ctrl_ptr->theme.font;

    return (result);
}

/*****************************************************************************/
//  Description : get button height by width,include border,text etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonCtrlGetHeightByWidth(
                                         CTRLBASE_OBJ_T     *base_ctrl_ptr,  //in:
                                         uint16             width,      //in:
                                         uint16             *height_ptr //in/out:
                                         )
{
    BOOLEAN                 result = TRUE;
    BOOLEAN                 is_border = TRUE;
    uint16                  font_height = 0;
    uint16                  line_height = 0;
    uint16                  bg_height = 0;
    MMI_STRING_T            button_str = {0};
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = (CTRLBUTTON_OBJ_T*)base_ctrl_ptr;

    //button is exist border
    if ((!button_ctrl_ptr->is_permit_border) || 
        (GUI_BORDER_NONE == button_ctrl_ptr->theme.border.type))
    {
        is_border = FALSE;
    }

    // 获取字符串
    button_str = ButtonGetTextInfo(button_ctrl_ptr);

    //get font height
    font_height = GUI_GetFontHeight(button_ctrl_ptr->theme.font.font,UNICODE_HANZI);

    //get line height
    line_height = (uint16)(font_height + (button_ctrl_ptr->theme.margin_space<<1));
    *height_ptr = line_height;

    //add border
    if (is_border)
    {
        *height_ptr = (uint16)(*height_ptr + (button_ctrl_ptr->theme.border.width << 1));
    }
    
    //get bg
    bg_height = VTLBUTTON_GetButtonBgHeight(button_ctrl_ptr);

    //has string
    if (0 < button_str.wstr_len)
    {
        // 上图下文的情况
        if (GUIBUTTON_STYLE_UIDT == button_ctrl_ptr->style
            && GUI_BG_IMG == button_ctrl_ptr->release.fg.bg_type)
        {
            uint16  image_height = 0;

            GUIRES_GetImgWidthHeight(
                PNULL, &image_height,
                button_ctrl_ptr->release.fg.img_id,
                button_ctrl_ptr->win_handle);

            *height_ptr += (image_height + button_ctrl_ptr->theme.image_text_space);
        }

        //get button height
        *height_ptr = (uint16)MAX(*height_ptr,bg_height);
    }
    else if (0 < bg_height)
    {
        *height_ptr = bg_height;
    }

    GUI_INVALID_PARAM(width);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
//  Description : set button display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonCtrlSetDisplayRect(
                                       CTRLBASE_OBJ_T       *base_ctrl_ptr,  //in:
                                       const GUI_RECT_T     *rect_ptr,  //in:
                                       BOOLEAN              is_update   //in:
                                       )
{
    BOOLEAN                 result = TRUE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    button_ctrl_ptr = (CTRLBUTTON_OBJ_T*)base_ctrl_ptr;

    //set display rect
    base_ctrl_ptr->display_rect = *rect_ptr;

    if (is_update)
    {
        //update button
        ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_NONE);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get button background or foreground
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ButtonCtrlGetBgOrFg(
                           CTRLBUTTON_OBJ_T     *button_ctrl_ptr,   //in:
                           GUI_BG_T             *bg_ptr,            //in/out:
                           BOOLEAN              is_bg               //in:
                           )
{
    if (is_bg)
    {
        if (button_ctrl_ptr->is_gray)
        {
            *bg_ptr = button_ctrl_ptr->gray.bg;
        }
        else
        {
            if ((GUIBUTTON_RUNSHEEN_ANIM != button_ctrl_ptr->theme.anim_type)
                && (button_ctrl_ptr->is_pressed
//#ifdef MMI_PDA_SUPPORT
                    || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()
                        && button_ctrl_ptr->is_active
                        )
//#endif
                    )
                )
            {
                *bg_ptr = button_ctrl_ptr->pressed.bg;
            }
            else
            {
                *bg_ptr = button_ctrl_ptr->release.bg;
            }
        }
    }
    else
    {
        //set fg
        if (button_ctrl_ptr->is_gray)
        {
            *bg_ptr = button_ctrl_ptr->gray.fg;
        }
        else
        {
            if ((GUIBUTTON_RUNSHEEN_ANIM != button_ctrl_ptr->theme.anim_type)
                && (button_ctrl_ptr->is_pressed
//#ifdef MMI_PDA_SUPPORT
                    || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()
                        && button_ctrl_ptr->is_active
                        )
//#endif
                    )
                )
            {
                *bg_ptr = button_ctrl_ptr->pressed.fg;
            }
            else
            {
                *bg_ptr = button_ctrl_ptr->release.fg;
            }
        }
    }

#if 0
    if (PNULL != bg_ptr)
    {
        switch (button_ctrl_ptr->type)
        {
        case GUIBUTTON_TYPE_RADIO:
            //if (is_bg)
            //{
            //    bg_ptr->bg_type = GUI_BG_IMG;
            //    if (button_ctrl_ptr->is_gray)
            //    {
            //        bg_ptr->img_id = button_ctrl_ptr->theme.radio.gray;
            //    }
            //    else
            //    {
            //        if (button_ctrl_ptr->is_select)
            //        {
            //            bg_ptr->img_id = button_ctrl_ptr->theme.radio.select;
            //        }
            //        else
            //        {
            //            bg_ptr->img_id = button_ctrl_ptr->theme.radio.unselect;
            //        }
            //    }
            //}
            //else
            //{
            //    bg_ptr->bg_type = GUI_BG_NONE;
            //}
            break;

        default:
            if (is_bg)
            {
                if (button_ctrl_ptr->is_gray)
                {
                    *bg_ptr = button_ctrl_ptr->gray.bg;
                }
                else
                {
                    if ((GUIBUTTON_RUNSHEEN_ANIM != button_ctrl_ptr->theme.anim_type)
                        && (button_ctrl_ptr->is_pressed
//#ifdef MMI_PDA_SUPPORT
                            || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()
                                && button_ctrl_ptr->is_active
                                )
//#endif
                            )
                        )
                    {
                        *bg_ptr = button_ctrl_ptr->pressed.bg;
                    }
                    else
                    {
                        *bg_ptr = button_ctrl_ptr->release.bg;
                    }
                }
            }
            else
            {
                //set fg
                if (button_ctrl_ptr->is_gray)
                {
                    *bg_ptr = button_ctrl_ptr->gray.fg;
                }
                else
                {
                    if ((GUIBUTTON_RUNSHEEN_ANIM != button_ctrl_ptr->theme.anim_type)
                        && (button_ctrl_ptr->is_pressed
//#ifdef MMI_PDA_SUPPORT
                            || (MMITHEME_TRIGGER_MODE_KEY == MMITHEME_GetTiggerMode()
                                && button_ctrl_ptr->is_active
                                )
//#endif
                            )
                        )
                    {
                        *bg_ptr = button_ctrl_ptr->pressed.fg;
                    }
                    else
                    {
                        *bg_ptr = button_ctrl_ptr->release.fg;
                    }
                }
            }
            break;
        }
    }
#endif
}

/*****************************************************************************/
//  Description : get button bg height
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 ButtonCtrlGetBgHeight(
                                CTRLBUTTON_OBJ_T    *button_ctrl_ptr
                                )
{
    uint16          bg_height = 0;
    uint16          pressed_bg_height = 0;
    GUI_BG_T        *bg_ptr = PNULL;
    GUI_BG_T        *fg_ptr = PNULL;

    if (button_ctrl_ptr->is_gray)
    {
        bg_ptr = &button_ctrl_ptr->gray.bg;
        fg_ptr = &button_ctrl_ptr->gray.fg;
    
        //get gray bg height
        bg_height = GetButtonBgFgHeight(bg_ptr,fg_ptr,button_ctrl_ptr);
    }
    else
    {
        bg_ptr = &button_ctrl_ptr->release.bg;
        fg_ptr = &button_ctrl_ptr->release.fg;

        //get release bg height
        bg_height = GetButtonBgFgHeight(bg_ptr,fg_ptr,button_ctrl_ptr);

        bg_ptr = &button_ctrl_ptr->pressed.bg;
        fg_ptr = &button_ctrl_ptr->pressed.fg;

        //get pressed bg height
        pressed_bg_height = GetButtonBgFgHeight(bg_ptr,fg_ptr,button_ctrl_ptr);

        bg_height = (uint16)MAX(pressed_bg_height,bg_height);
    }

    return bg_height;
}

/*****************************************************************************/
//  Description : display button control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ButtonDraw(
                     CTRLBUTTON_OBJ_T           *button_ctrl_ptr,//in:
                     GUIBUTTON_OWNDRAW_STATE_E  draw_state
                     )
{
    GUI_RECT_T              cross_rect = {0};
    GUI_RECT_T              button_rect = {0};
    GUIBUTTON_OWNER_DRAW_T  owner_draw = {0};
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*)button_ctrl_ptr;

    if ((PNULL != button_ctrl_ptr) &&
        (VTLBASE_GetVisible((CTRLBASE_OBJ_T*)button_ctrl_ptr)) &&
        (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,base_ctrl_ptr->rect)))
    {
        //display bg
        ButtonDrawBg(button_ctrl_ptr);

        //display fg
        ButtonDrawFg(button_ctrl_ptr);

        if (button_ctrl_ptr->is_permit_border)
        {
            //display border
            GUI_DisplayBorder(base_ctrl_ptr->rect,
                base_ctrl_ptr->display_rect,
                &button_ctrl_ptr->theme.border,
                &base_ctrl_ptr->lcd_dev_info);
        }

        //display text
        ButtonDrawText(button_ctrl_ptr);

        //owner draw
        if (PNULL != button_ctrl_ptr->OwnerDrawFunc)
        {
            //get button rect
            button_rect = GetButtonRect(button_ctrl_ptr);
            if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,button_rect))
            {
                owner_draw.display_x    = button_ctrl_ptr->bg_display_x;
                owner_draw.display_y    = button_ctrl_ptr->bg_display_y;
                owner_draw.fg_display_x = button_ctrl_ptr->fg_display_x;
                owner_draw.fg_display_y = button_ctrl_ptr->fg_display_y;
                owner_draw.display_rect = cross_rect;
                owner_draw.ctrl_handle  = base_ctrl_ptr->handle;
                owner_draw.draw_state   = draw_state;
                owner_draw.lcd_dev      = base_ctrl_ptr->lcd_dev_info;

                button_ctrl_ptr->OwnerDrawFunc(&owner_draw);
            }
        }
    }
}

/*****************************************************************************/
//  Description : display button background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ButtonDrawBg(
                       CTRLBUTTON_OBJ_T     *button_ctrl_ptr//in:
                       )
{
    uint16              img_width = 0;
    uint16              img_height = 0;
    GUI_BG_T            button_bg = {0};
    GUI_RECT_T          button_rect = {0};
    GUI_RECT_T          img_rect = {0};
    GUI_RECT_T          lcd_rect = {0};
    GUI_RECT_T          image_display_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)button_ctrl_ptr;

    //set bg
    VTLBUTTON_GetBgOrFg(button_ctrl_ptr,&button_bg,TRUE);

    //get button string rect
    button_rect = GetButtonRect(button_ctrl_ptr);

    //set display x and y
    button_ctrl_ptr->bg_display_x = button_rect.left;
    button_ctrl_ptr->bg_display_y = button_rect.top;

    if (GUI_IntersectRect(&lcd_rect,button_rect,base_ctrl_ptr->display_rect))
    {
        image_display_rect = button_rect;
        
        //adjust bg img,图片可拉伸,不需要调整rect
        if ((GUI_BG_IMG == button_bg.bg_type) && 
            (!GUIRES_IsSabm(button_bg.img_id,button_ctrl_ptr->win_handle)))
        {
            //get image width and height
            GUIRES_GetImgWidthHeight(&img_width,&img_height,button_bg.img_id,button_ctrl_ptr->win_handle);

            //set init display x and y and adjust display position
            GUIANIM_AdjustDisplayPosition(&button_ctrl_ptr->bg_display_x,
                    &button_ctrl_ptr->bg_display_y,
                    img_width,
                    img_height,
                    button_rect,
                    GUIANIM_ALIGN_HVMIDDLE);

            //set image display rect
            image_display_rect.left   = (int16)MAX(button_ctrl_ptr->bg_display_x,button_rect.left);
            image_display_rect.top    = (int16)MAX(button_ctrl_ptr->bg_display_y,button_rect.top);
            image_display_rect.right  = (int16)MIN((image_display_rect.left + img_width - 1),button_rect.right);
            image_display_rect.bottom = (int16)MIN((image_display_rect.top + img_height - 1),button_rect.bottom);
        }

        //display bg or fg
        if (GUI_IntersectRect(&lcd_rect,base_ctrl_ptr->display_rect,image_display_rect))
        {
            //image rect
            img_rect.left   = (int16)(lcd_rect.left - button_ctrl_ptr->bg_display_x);
            img_rect.top    = (int16)(lcd_rect.top - button_ctrl_ptr->bg_display_y);
            img_rect.right  = (int16)(lcd_rect.right - button_ctrl_ptr->bg_display_x);
            img_rect.bottom = (int16)(lcd_rect.bottom - button_ctrl_ptr->bg_display_y);

            //set bg display info
            bg_display.is_transparent = button_ctrl_ptr->is_transparent;
            bg_display.rect           = base_ctrl_ptr->rect;
            bg_display.display_rect   = lcd_rect;
            bg_display.img_rect       = img_rect;
            bg_display.ctrl_handle    = base_ctrl_ptr->handle;
            bg_display.win_handle     = button_ctrl_ptr->win_handle;

            if (GUI_BG_IMG == button_bg.bg_type)
            {
                IMG_EnableTransparentColor(TRUE);
            }

            GUI_DisplayBg(
                &button_bg,
                &bg_display,
                &base_ctrl_ptr->lcd_dev_info);

            IMG_EnableTransparentColor(FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : display button foreground
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ButtonDrawFg(
                       CTRLBUTTON_OBJ_T     *button_ctrl_ptr//in:
                       )
{
    uint16          img_width = 0;
    uint16          img_height = 0;
    GUI_BG_T        button_fg = {0};
    GUI_RECT_T      image_rect = {0};
    GUI_RECT_T      img_rect = {0};
    GUI_RECT_T      lcd_rect = {0};
    GUI_RECT_T      image_display_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)button_ctrl_ptr;

    //set fg
    VTLBUTTON_GetBgOrFg(button_ctrl_ptr,&button_fg,FALSE);

    //get button string rect
    CalcImageTextRect(button_ctrl_ptr, &image_rect, PNULL);

    //set display x and y
    button_ctrl_ptr->fg_display_x = image_rect.left;
    button_ctrl_ptr->fg_display_y = image_rect.top;

    if (GUI_IntersectRect(&lcd_rect,image_rect,base_ctrl_ptr->display_rect))
    {
        switch (button_fg.bg_type)
        {
        case GUI_BG_NONE:
            break;

        case GUI_BG_IMG:
            //get image width and height
            GUIRES_GetImgWidthHeight(&img_width,&img_height,button_fg.img_id,button_ctrl_ptr->win_handle);

            //set init display x and y and adjust display position
            GUIANIM_AdjustDisplayPosition(
                    &button_ctrl_ptr->fg_display_x,
                    &button_ctrl_ptr->fg_display_y,
                    img_width,
                    img_height,
                    image_rect,
                    GUIANIM_ALIGN_HVMIDDLE);

            //set image display rect
            image_display_rect.left   = (int16)MAX(button_ctrl_ptr->fg_display_x,image_rect.left);
            image_display_rect.top    = (int16)MAX(button_ctrl_ptr->fg_display_y,image_rect.top);
            image_display_rect.right  = (int16)MIN((image_display_rect.left + img_width - 1),image_rect.right);
            image_display_rect.bottom = (int16)MIN((image_display_rect.top + img_height - 1),image_rect.bottom);

            //display fg
            if (GUI_IntersectRect(&lcd_rect,base_ctrl_ptr->display_rect,image_display_rect))
            {
                //image rect
                img_rect.left   = (int16)(lcd_rect.left - button_ctrl_ptr->fg_display_x);
                img_rect.top    = (int16)(lcd_rect.top - button_ctrl_ptr->fg_display_y);
                img_rect.right  = (int16)(lcd_rect.right - button_ctrl_ptr->fg_display_x);
                img_rect.bottom = (int16)(lcd_rect.bottom - button_ctrl_ptr->fg_display_y);
                
                IMG_EnableTransparentColor(TRUE);
                GUIRES_DisplayImg(PNULL,
                    &lcd_rect,
                    &img_rect,
                    button_ctrl_ptr->win_handle,
                    button_fg.img_id,
                    (const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info);
                IMG_EnableTransparentColor(FALSE);
            }
            break;

        case GUI_BG_COLOR:
            GUI_FillRect(&base_ctrl_ptr->lcd_dev_info,
                lcd_rect,
                button_fg.color);
            break;

        default:
            //SCI_TRACE_LOW:"ButtonDrawFg:bg_type %d is error !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIBUTTON_898_112_2_18_3_15_10_56,(uint8*)"d",button_fg.bg_type);
            break;
        }
    }
}

/*****************************************************************************/
//  Description : button destruct
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ButtonFreeText(
                             CTRLBUTTON_OBJ_T    *button_ctrl_ptr
                             )
{
    BOOLEAN             result = TRUE;

    if (PNULL != button_ctrl_ptr)
    {
        //free string
        if (PNULL != button_ctrl_ptr->text_info.str_info.wstr_ptr)
        {
            SCI_FREE(button_ctrl_ptr->text_info.str_info.wstr_ptr);
            button_ctrl_ptr->text_info.str_info.wstr_len = 0;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : display button text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_STRING_T ButtonGetTextInfo(
                                     CTRLBUTTON_OBJ_T       *button_ctrl_ptr
                                     )
{
    MMI_STRING_T    str_info = {0};

    if (PNULL == button_ctrl_ptr)
    {
        return str_info;
    }

    if (GUISTR_TEXT_ID == button_ctrl_ptr->text_info.text_type)
    {
        MMITHEME_GetResText( button_ctrl_ptr->text_info.text_id, button_ctrl_ptr->win_handle, &str_info );
    }
    else if (GUISTR_TEXT_BUFFER == button_ctrl_ptr->text_info.text_type)
    {
        //get string
        str_info.wstr_ptr = button_ctrl_ptr->text_info.str_info.wstr_ptr;
        str_info.wstr_len = button_ctrl_ptr->text_info.str_info.wstr_len;
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }

    return str_info;
}

/*****************************************************************************/
//  Description : display button text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ButtonDrawText(
                         CTRLBUTTON_OBJ_T       *button_ctrl_ptr
                         )
{
    GUI_RECT_T      text_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    MMI_STRING_T    button_str = {0};
    GUISTR_STYLE_T  str_style = {0};
    GUISTR_STATE_T  str_state = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)button_ctrl_ptr;

    button_str = ButtonGetTextInfo(button_ctrl_ptr);

    if (PNULL != button_ctrl_ptr && 0 < button_str.wstr_len)
    {
        //get button string rect
        CalcImageTextRect(button_ctrl_ptr, PNULL, &text_rect);

        //except margin
        text_rect.left  = (uint16)(text_rect.left + button_ctrl_ptr->theme.margin_space);
        text_rect.right = (uint16)(text_rect.right - button_ctrl_ptr->theme.margin_space);

        //get crossed rect
        if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,text_rect))
        {
            //set button string
           // button_str.wstr_ptr = button_ctrl_ptr->str_ptr;
           // button_str.wstr_len = button_ctrl_ptr->str_len;

            //set string style
            str_style.font       = button_ctrl_ptr->theme.font.font;
            if (button_ctrl_ptr->is_gray)
            {
                str_style.font_color = MMITHEME_GetGrayColor();
            }
            else
            {
                str_style.font_color = button_ctrl_ptr->theme.font.color;
            }

            str_style.align      = button_ctrl_ptr->theme.text_align;
            str_style.char_space = button_ctrl_ptr->theme.char_space;

            if (GUIBUTTON_STYLE_LIRTM == button_ctrl_ptr->style
                || GUIBUTTON_STYLE_LIRTL == button_ctrl_ptr->style)
            {
                str_style.align  = ALIGN_LVMIDDLE;
            }
            str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
            if ((ALIGN_LEFT == button_ctrl_ptr->theme.text_align) ||
                (ALIGN_TOP == button_ctrl_ptr->theme.text_align) ||
                (ALIGN_BOTTOM == button_ctrl_ptr->theme.text_align) ||
                (ALIGN_VMIDDLE == button_ctrl_ptr->theme.text_align) ||
                (ALIGN_LVMIDDLE == button_ctrl_ptr->theme.text_align))
            {
                str_state = str_state | GUISTR_STATE_ALIGN_BY_DIR;
            }

            GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO*)&base_ctrl_ptr->lcd_dev_info,
                (const GUI_RECT_T*)&text_rect,
                (const GUI_RECT_T*)&cross_rect,
                &button_str,
                &str_style,
                str_state,
                GUISTR_TEXT_DIR_AUTO);
        }
    }
}

/*****************************************************************************/
//  Description : get button rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetButtonRect(
                               CTRLBUTTON_OBJ_T      *button_ctrl_ptr
                               )
{
    BOOLEAN         is_border = TRUE;
    GUI_RECT_T      button_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)button_ctrl_ptr;

    //button is exist border
    if ((!button_ctrl_ptr->is_permit_border) || 
        (GUI_BORDER_NONE == button_ctrl_ptr->theme.border.type))
    {
        is_border = FALSE;
    }

    if (is_border)
    {
        button_rect.left   = (int16)(base_ctrl_ptr->rect.left + button_ctrl_ptr->theme.border.width);
        button_rect.right  = (int16)(base_ctrl_ptr->rect.right - button_ctrl_ptr->theme.border.width);
        button_rect.top    = (int16)(base_ctrl_ptr->rect.top + button_ctrl_ptr->theme.border.width);
        button_rect.bottom = (int16)(base_ctrl_ptr->rect.bottom - button_ctrl_ptr->theme.border.width);
    }
    else
    {
        button_rect = base_ctrl_ptr->rect;
    }

    return (button_rect);
}

/*****************************************************************************/
//  Description : calculate the foreground image and text region
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CalcImageTextRect(
                             CTRLBUTTON_OBJ_T       *button_ctrl_ptr,
                             GUI_RECT_T             *image_rect_ptr,        // [out] image dst rect
                             GUI_RECT_T             *text_rect_ptr          // [out] text dst rect
                             )
{
    GUI_RECT_T      button_rect = {0};
    uint16          image_height = 0;
    uint16          image_width = 0;

    button_rect = GetButtonRect(button_ctrl_ptr);

    if (PNULL != image_rect_ptr)
    {
        *image_rect_ptr = button_rect;
    }

    if (PNULL != text_rect_ptr)
    {
        *text_rect_ptr = button_rect;
    }

    // 上图下文的情况
    if (GUIBUTTON_STYLE_UIDT == button_ctrl_ptr->style
        && GUI_BG_IMG == button_ctrl_ptr->release.fg.bg_type)
    {
        uint8   font_height = 0;
        int16   start_top = 0;

        GUIRES_GetImgWidthHeight(
            PNULL, &image_height,
            button_ctrl_ptr->release.fg.img_id,
            button_ctrl_ptr->win_handle);

        font_height = GUI_GetFontHeight(button_ctrl_ptr->theme.font.font, UNICODE_HANZI);

        if (button_rect.bottom - button_rect.top + 1 > image_height + font_height + button_ctrl_ptr->theme.image_text_space + (button_ctrl_ptr->theme.margin_space<<1))
        {
            start_top = (button_rect.bottom - button_rect.top + 1 - (image_height + font_height + button_ctrl_ptr->theme.image_text_space + (button_ctrl_ptr->theme.margin_space<<1))) / 2;
        }

        if (PNULL != image_rect_ptr)
        {
            *image_rect_ptr = button_rect;

            image_rect_ptr->top += start_top;
            image_rect_ptr->bottom = image_rect_ptr->top + image_height - 1;

            // 超出button区域的情况
            if (image_rect_ptr->bottom > button_rect.bottom)
            {
                image_rect_ptr->bottom = button_rect.bottom;
            }
        }

        if (PNULL != text_rect_ptr)
        {
            *text_rect_ptr = button_rect;

            text_rect_ptr->top = button_rect.top + start_top + image_height + button_ctrl_ptr->theme.image_text_space;

            // 顶超底的情况
            if (text_rect_ptr->top > text_rect_ptr->bottom)
            {
                text_rect_ptr->top = text_rect_ptr->bottom;
            }
        }
    }
    else if (GUIBUTTON_STYLE_LIRTM == button_ctrl_ptr->style
        && GUI_BG_IMG == button_ctrl_ptr->release.fg.bg_type)
    {
        GUIRES_GetImgWidthHeight(
            &image_width, PNULL,
            button_ctrl_ptr->release.fg.img_id,
            button_ctrl_ptr->win_handle);

        if (PNULL != image_rect_ptr)
        {
            *image_rect_ptr = button_rect;
            image_rect_ptr->right = (button_rect.left + button_rect.right) / 2 - button_ctrl_ptr->theme.image_space;
            image_rect_ptr->left = image_rect_ptr->right - image_width + 1;

            // 超出button区域的情况
            if (image_rect_ptr->left < button_rect.left)
            {
                image_rect_ptr->left = button_rect.left;
            }
        }

        if (PNULL != text_rect_ptr)
        {
            *text_rect_ptr = button_rect;

            text_rect_ptr->left = (button_rect.left + button_rect.right) / 2;
            text_rect_ptr->top += button_ctrl_ptr->theme.margin_space;
        }
    }
    else if (GUIBUTTON_STYLE_LIRTL == button_ctrl_ptr->style
        && GUI_BG_IMG == button_ctrl_ptr->release.fg.bg_type)
    {
        GUIRES_GetImgWidthHeight(
            &image_width, PNULL,
            button_ctrl_ptr->release.fg.img_id,
            button_ctrl_ptr->win_handle);

        if (PNULL != image_rect_ptr)
        {
            *image_rect_ptr = button_rect;
            image_rect_ptr->left += button_ctrl_ptr->theme.image_space;
            image_rect_ptr->right = image_rect_ptr->left + image_width - 1;

            // 超出button区域的情况
            if (image_rect_ptr->right > button_rect.right)
            {
                image_rect_ptr->right = button_rect.right;
            }
        }

        if (PNULL != text_rect_ptr)
        {
            *text_rect_ptr = button_rect;

            text_rect_ptr->left += image_width + 2*button_ctrl_ptr->theme.image_space;
            text_rect_ptr->top += button_ctrl_ptr->theme.margin_space;

            // 顶超底的情况
            if (text_rect_ptr->left > text_rect_ptr->right)
            {
                text_rect_ptr->left = text_rect_ptr->right;
            }
        }
    }
}

#ifdef TOUCH_PANEL_SUPPORT 
/*****************************************************************************/
//  Description : handle button tp down message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ButtonHandleTpDown(
                                      CTRLBUTTON_OBJ_T      *button_ctrl_ptr,
                                      DPARAM                param
                                      )
{
    GUI_RECT_T           cross_rect  = {0};
    MMI_RESULT_E         result      = MMI_RESULT_FALSE;
    GUI_SHEEN_INPUT_T    sheen_input = {0};
    CTRLBASE_OBJ_T       *base_ctrl_ptr = (CTRLBASE_OBJ_T*)button_ctrl_ptr;

    GUI_INVALID_PARAM(param);/*lint !e522*/

    if ((!button_ctrl_ptr->is_gray) &&
        (!button_ctrl_ptr->is_pressed))
    {
        MMITHEME_UpdateRect();

        button_ctrl_ptr->is_send_long = FALSE;

        //set pressed
        button_ctrl_ptr->is_pressed = TRUE;

        //set select
        VTLBUTTON_TpDown(button_ctrl_ptr);
        
        if (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,base_ctrl_ptr->rect))
        {           
            if (GUIBUTTON_RUNSHEEN_ANIM == button_ctrl_ptr->theme.anim_type)
            {
                //set sheen
                sheen_input.lcd_dev         = base_ctrl_ptr->lcd_dev_info;
                sheen_input.rect            = cross_rect;
#if defined(ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR) //Bug 2060397
                sheen_input.center_depth    = GUI_SHEEN_DEPTH_LOW; //Bug 1594714
#else
                sheen_input.center_depth    = GUI_SHEEN_DEPTH_HIGH;
#endif
                sheen_input.side_depth      = GUI_SHEEN_DEPTH_LOW;

                //display sheen
                button_ctrl_ptr->sheen_ptr = GUI_DisplaySheen(&sheen_input);
            }
            else if (GUIBUTTON_POPBLINK_ANIM == button_ctrl_ptr->theme.anim_type)
            {
                //display pop
                if(CreateAndAppendPopLayer(button_ctrl_ptr))
                {
                    ButtonDrawPopAnim(button_ctrl_ptr, 0);//tp up的过程只播到第一帧最亮
                }
            }
            else
            {
                //display button
                ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_TP_DOWN);
            }

            CTRLMSG_PostNotify(base_ctrl_ptr->handle, MSG_NOTIFY_BUTTON_DOWN);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle button tp up message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ButtonHandleTpUp(
                                    CTRLBUTTON_OBJ_T        *button_ctrl_ptr,
                                    DPARAM                  param
                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    GUI_RECT_T      cross_rect = {0};
    GUI_POINT_T     tp_point = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)button_ctrl_ptr;

    if (button_ctrl_ptr->is_pressed)
    {
        //set release
        button_ctrl_ptr->is_pressed = FALSE;

        if (!button_ctrl_ptr->is_gray)
        {
            //set tp press point
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);

            //Display Pop Anim
            if (GUIBUTTON_POPBLINK_ANIM == button_ctrl_ptr->theme.anim_type)
            {
                ButtonDrawPopAnim(button_ctrl_ptr, button_ctrl_ptr->theme.pop_info.total_frame);
                ReleasePopBlinkLayer(button_ctrl_ptr);
            }
            //reset button sheen
            else if (!ResetButtonSheen(button_ctrl_ptr))
            {
                ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_TP_UP);
            }

            if ((!MMK_GET_TP_SLIDE(param)) &&
                (GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,base_ctrl_ptr->rect)))
            {
                if (GUI_PointIsInRect(tp_point,cross_rect))
                {
                    CTRLMSG_PostNotify(base_ctrl_ptr->handle, MSG_NOTIFY_BUTTON_UP);
                    
                    if (!button_ctrl_ptr->is_send_long)
                    {
                        if (PNULL != button_ctrl_ptr->callback_func)
                        {
                            button_ctrl_ptr->callback_func();
                        }
                        else if (PNULL != button_ctrl_ptr->callback_func_ext)
                        {
                            button_ctrl_ptr->callback_func_ext(base_ctrl_ptr->handle);
                        }
                        else
                        {
                            //notify parent control
                            CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_PENOK);
                        }
                    }
                }
                else
                {
                    VTLBUTTON_TpMoveOut(button_ctrl_ptr);
                    ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_TP_UP);
                }
            }
        }        
    }
    
    return (result);
}
#endif

/*****************************************************************************/
//  Description : reset button sheen
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetButtonSheen(
                               CTRLBUTTON_OBJ_T         *button_ctrl_ptr
                               )
{
    BOOLEAN     result = FALSE;

    if (GUIBUTTON_RUNSHEEN_ANIM == button_ctrl_ptr->theme.anim_type)
    {
        //set release
        button_ctrl_ptr->is_pressed = FALSE;

        //release sheen
        if (PNULL != button_ctrl_ptr->sheen_ptr)
        {
            GUI_ReleaseSheen(button_ctrl_ptr->sheen_ptr,TRUE);
            button_ctrl_ptr->sheen_ptr = PNULL;
        }

        result = TRUE;
    }

    return (result);
}

#ifdef TOUCH_PANEL_SUPPORT 
/*****************************************************************************/
//  Description : handle button tp long message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ButtonHandleTpLong(
                                      CTRLBUTTON_OBJ_T      *button_ctrl_ptr,
                                      DPARAM                param
                                      )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    GUI_RECT_T      cross_rect = {0};
    GUI_POINT_T     tp_point = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)button_ctrl_ptr;

    if ((!button_ctrl_ptr->is_gray) &&
        (button_ctrl_ptr->is_handle_long) &&
        (button_ctrl_ptr->is_pressed))
    {
        //set tp press point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        if ((GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,base_ctrl_ptr->rect)) && 
            (GUI_PointIsInRect(tp_point,cross_rect)))
        {
            button_ctrl_ptr->is_send_long = TRUE;

            //notify parent control
            CTRLMSG_PostNotify(base_ctrl_ptr->handle,MSG_NOTIFY_PENLONGOK);
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : handle button tp move message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ButtonHandleTpMove(
                                      CTRLBUTTON_OBJ_T      *button_ctrl_ptr,
                                      DPARAM                param
                                      )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    GUI_RECT_T      cross_rect = {0};
    GUI_POINT_T     tp_point = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)button_ctrl_ptr;

    if (button_ctrl_ptr->is_pressed)
    {
        //set tp press point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        if ((GUI_IntersectRect(&cross_rect,base_ctrl_ptr->display_rect,base_ctrl_ptr->rect)) && 
            (!GUI_PointIsInRect(tp_point,cross_rect)))
        {
            //set release
            button_ctrl_ptr->is_pressed = FALSE;
            VTLBUTTON_TpMoveOut(button_ctrl_ptr);

            if (!button_ctrl_ptr->is_gray)
            {
                //Display Pop Anim
                if (GUIBUTTON_POPBLINK_ANIM == button_ctrl_ptr->theme.anim_type)
                {
                    ButtonDrawPopAnim(button_ctrl_ptr, button_ctrl_ptr->theme.pop_info.total_frame);
                    ReleasePopBlinkLayer(button_ctrl_ptr);
                }
                //reset button sheen
                else if (!ResetButtonSheen(button_ctrl_ptr))
                {
                    ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_TP_MOVE);
                }

                CTRLMSG_PostNotify(base_ctrl_ptr->handle, MSG_NOTIFY_BUTTON_UP);
            }

        }
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description : get button pointer by control id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLBUTTON_OBJ_T* GetButtonPtr(
                                     MMI_HANDLE_T   ctrl_handle
                                     )
{
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT(ButtonTypeOf(ctrl_ptr));/*assert verified*/
    }
    
    return (CTRLBUTTON_OBJ_T*)ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ButtonTypeOf(
                           CTRLBASE_OBJ_T   *ctrl_ptr
                           )
{
    return TYPEMNG_IsTypeOf(ctrl_ptr, CTRL_BUTTON_TYPE);
}

/*****************************************************************************/
//  Description : get button bg and fg max height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetButtonBgFgHeight(
                                 GUI_BG_T               *bg_ptr,        //in:
                                 GUI_BG_T               *fg_ptr,        //in:
                                 CTRLBUTTON_OBJ_T       *button_ctrl_ptr//in:
                                 )
{
    uint16      bg_height = 0;

    if (PNULL != bg_ptr)
    {
        switch (bg_ptr->bg_type)
        {
        case GUI_BG_NONE:
        case GUI_BG_COLOR:
            if ((PNULL != fg_ptr) &&
                (GUI_BG_IMG == fg_ptr->bg_type))
            {
                GUIRES_GetImgWidthHeight(PNULL,&bg_height,fg_ptr->img_id,button_ctrl_ptr->win_handle);
            }
            break;

        case GUI_BG_IMG:
            GUIRES_GetImgWidthHeight(PNULL,&bg_height,bg_ptr->img_id,button_ctrl_ptr->win_handle);
            break;

        default:
            break;
        }
    }

    return (bg_height);
}

/*****************************************************************************/
//  Description : display pop effect
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ButtonDrawPopAnim(
                          CTRLBUTTON_OBJ_T      *button_ctrl_ptr,
                          int32                 total_dis_frame
                          )
{       
    GUIBUTTON_POP_DRAW_INPUT_T pop_info     = {0};
    GUI_RECT_T            pop_dis_rect = {0};
    
    if (PNULL == button_ctrl_ptr)
    {
        return;
    }
   
    //get pop center point
    pop_info.centre_point.x     = button_ctrl_ptr->theme.pop_info.radius;
    pop_info.centre_point.y     = button_ctrl_ptr->theme.pop_info.radius;
    pop_info.centre_point.x_dec = 0;
    pop_info.centre_point.y_dec = 0;
    
    //get pop_dis_rect
    pop_dis_rect = CalculatePopLayerRelativeRect(button_ctrl_ptr);
    if (GUI_IsInvalidRect(pop_dis_rect))
    {
        return;
    }
    
    //get dis_rect width and height
    pop_info.show_size.w = (uint16)(pop_dis_rect.right - pop_dis_rect.left + 1);
    pop_info.show_size.h = (uint16)(pop_dis_rect.bottom - pop_dis_rect.top + 1);
#ifdef UI_MULTILAYER_SUPPORT	//支持多图层    
    //get pop buffer
    pop_info.pop_buf_ptr = UILAYER_GetLayerBufferPtr(&button_ctrl_ptr->pop_layer_info);
#endif 
    if(PNULL== pop_info.pop_buf_ptr)
    {
        return;
    }
    
    pop_info.total_dis_frame = total_dis_frame;
    
    ButtonDrawPopBlinkingEffect(
        button_ctrl_ptr,
        &pop_info);   
    
    return;
}
 
/*****************************************************************************/
//  Description : Drawn PopBlinking Effect
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ButtonDrawPopBlinkingEffect(
                                 CTRLBUTTON_OBJ_T           *button_ctrl_ptr,
                                 GUIBUTTON_POP_DRAW_INPUT_T *pop_info_ptr
                                 )
{
    int32 cur_frame = 0;
    
    if((PNULL == button_ctrl_ptr)||(PNULL == pop_info_ptr))
    {
        return;
    }
    
    for (cur_frame = 0; cur_frame <= pop_info_ptr->total_dis_frame; cur_frame++)
    {
#ifdef UI_MULTILAYER_SUPPORT        
        if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(&(button_ctrl_ptr->pop_layer_info)))
        {
            UILAYER_Clear(&(button_ctrl_ptr->pop_layer_info));              
            PopBlinking(cur_frame, button_ctrl_ptr->theme.pop_info.total_frame, &(pop_info_ptr->centre_point), button_ctrl_ptr->theme.pop_info.radius, button_ctrl_ptr->theme.pop_info.filled_color, pop_info_ptr->show_size, S3D_DATA_ARGB888, (uint32*)pop_info_ptr->pop_buf_ptr);
        }
        else if(DATA_TYPE_RGB565 == UILAYER_GetLayerColorType(&(button_ctrl_ptr->pop_layer_info)))
        {   
            UILAYER_Clear(&(button_ctrl_ptr->pop_layer_info));
            PopBlinking(cur_frame, button_ctrl_ptr->theme.pop_info.total_frame, &(pop_info_ptr->centre_point), button_ctrl_ptr->theme.pop_info.radius, button_ctrl_ptr->theme.pop_info.filled_color, pop_info_ptr->show_size, S3D_DATA_RGB565, (uint16*)pop_info_ptr->pop_buf_ptr);
        }
#endif        
        //update screen
        MMITHEME_UpdateRect();
        
        //sleep
#ifdef WIN32
        SCI_Sleep(20);
#else
        SCI_Sleep(30);
#endif
    }
}
/****************************************************************************/    
//  Description: Create And Append Pop Layer
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T CalculatePopLayerRelativeRect(
                                               CTRLBUTTON_OBJ_T     *button_ctrl_ptr
                                               )
{
    IMGREF_SIZE_T       button_size          = {0};
    GUI_POINT_T         centre_relative_rect = {0};
    GUI_RECT_T          pop_relative_rect    = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)button_ctrl_ptr;

    if (PNULL == button_ctrl_ptr)
    {
        return pop_relative_rect;
    }

    if (GUI_IsInvalidRect(base_ctrl_ptr->rect))
    {
        return pop_relative_rect;
    }

    //get button size 
    button_size.w  = base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left + 1;
    button_size.h  = base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top + 1;

    //get button centre point relative rect
    centre_relative_rect.x = base_ctrl_ptr->rect.left + button_size.w/2;
    centre_relative_rect.y = base_ctrl_ptr->rect.top + button_size.h/2;
    
    //calculate pop layer relative rect
    pop_relative_rect.left   = centre_relative_rect.x - button_ctrl_ptr->theme.pop_info.radius;
    pop_relative_rect.right  = centre_relative_rect.x + button_ctrl_ptr->theme.pop_info.radius;
    pop_relative_rect.top    = centre_relative_rect.y - button_ctrl_ptr->theme.pop_info.radius;
    pop_relative_rect.bottom = centre_relative_rect.y + button_ctrl_ptr->theme.pop_info.radius;

    return pop_relative_rect;        
}

/****************************************************************************/    
//  Description: Create And Append Pop Layer
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateAndAppendPopLayer(
                                      CTRLBUTTON_OBJ_T      *button_ctrl_ptr
                                      )
{
    BOOLEAN result = FALSE;
    GUI_RECT_T pop_layer_rect = {0};
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_APPEND_BLT_T append_layer_info = {0};
#endif
    if (PNULL == button_ctrl_ptr)
    {
        return result;
    }

    if (button_ctrl_ptr->theme.pop_info.is_default_radius)
    {
        SetDefaultPopEffectRadius(button_ctrl_ptr);
    }

    pop_layer_rect = CalculatePopLayerRelativeRect(button_ctrl_ptr);
    if (GUI_IsInvalidRect(pop_layer_rect))
    {
        return result;
    }   
#ifdef UI_MULTILAYER_SUPPORT
    //create pop layer
    result = CreatePopBlinkLayer(button_ctrl_ptr, &pop_layer_rect);

    append_layer_info.layer_level = UILAYER_LEVEL_NORMAL;
    append_layer_info.lcd_dev_info = button_ctrl_ptr->pop_layer_info;

    //append pop layer
    UILAYER_AppendBltLayer(&append_layer_info);
#endif
    return result;
}   

/****************************************************************************/    
//  Description: Create Pop Blink Layer
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePopBlinkLayer(
                                  CTRLBUTTON_OBJ_T      *button_ctrl_ptr,
                                  GUI_RECT_T            *pop_dis_rect
                                  )
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)button_ctrl_ptr;

    if ((PNULL == button_ctrl_ptr)||(PNULL == pop_dis_rect))
    {
        return result;
    }
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T create_info = {0};

        //init pop layer info
        create_info.lcd_id       = base_ctrl_ptr->lcd_dev_info.lcd_id;
        create_info.owner_handle = base_ctrl_ptr->handle;
        create_info.offset_x     = pop_dis_rect->left;
        create_info.offset_y     = pop_dis_rect->top;
        create_info.width        = pop_dis_rect->right - pop_dis_rect->left + 1;
        create_info.height       = pop_dis_rect->bottom - pop_dis_rect->top + 1;

        //create pop layer
        if(UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info, &(button_ctrl_ptr->pop_layer_info)))
        {
            UILAYER_SetLayerColorKey(&(button_ctrl_ptr->pop_layer_info), TRUE, UILAYER_TRANSPARENT_COLOR);
            UILAYER_Clear(&(button_ctrl_ptr->pop_layer_info));
            result = TRUE;
        }       
    }
#endif
    return result;
}

/****************************************************************************/    
//  Description :release pop layer
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void ReleasePopBlinkLayer( 
                                CTRLBUTTON_OBJ_T     *button_ctrl_ptr
                                )
{
    if (PNULL == button_ctrl_ptr)
    {
        return;
    }
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_RELEASELAYER(&(button_ctrl_ptr->pop_layer_info));
        button_ctrl_ptr->pop_layer_info.block_id = GUI_BLOCK_MAIN;
    }
#endif
}

/****************************************************************************/    
//  Description :Set Default Pop Effect Radius
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void SetDefaultPopEffectRadius( 
                                     CTRLBUTTON_OBJ_T       *button_ctrl_ptr
                                     )
{
    IMGREF_SIZE_T button_size = {0};
    GUI_RECT_T    button_rect = {0};
       
    if (PNULL == button_ctrl_ptr)
    {
        return;
    }
    
    //get button rect without border
    button_rect = GetButtonRect(button_ctrl_ptr);

    //get button size 
    button_size.w  = button_rect.right - button_rect.left + 1;
    button_size.h  = button_rect.bottom - button_rect.top + 1;

    //get default pop radius
    button_ctrl_ptr->theme.pop_info.radius = MIN(button_size.w, button_size.h)/2 -2;
}

/*****************************************************************************/
//  Description : reset button pop
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ResetButtonPop(
                             CTRLBUTTON_OBJ_T       *button_ctrl_ptr
                             )
{
    BOOLEAN     result = FALSE;

    if (GUIBUTTON_POPBLINK_ANIM == button_ctrl_ptr->theme.anim_type)
    {
        //set release
        button_ctrl_ptr->is_pressed = FALSE;

        //release pop
        ReleasePopBlinkLayer(button_ctrl_ptr);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : create dynamic button
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLBUTTON_CreateDynamic(
                                            MMI_WIN_ID_T            win_id,     //in:
                                            MMI_CTRL_ID_T           ctrl_id,    //in:
                                            GUIBUTTON_INIT_DATA_T   *init_ptr   //in:
                                            )
{
    MMI_CONTROL_CREATE_T    ctrl_create = {0};
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;
    MMI_HANDLE_T            handle = 0;

    //set control param
    ctrl_create.ctrl_id           = ctrl_id;
    ctrl_create.guid              = SPRD_GUI_BUTTON_ID;
    ctrl_create.init_data_ptr     = init_ptr;
    ctrl_create.parent_win_handle = win_id;

    //creat control
    button_ctrl_ptr = (CTRLBUTTON_OBJ_T*)MMK_CreateControl(&ctrl_create);
    if (PNULL != button_ctrl_ptr)
    {
        handle = button_ctrl_ptr->base_ctrl.handle;
    }

    return (handle);
}

/*****************************************************************************/
//  Description : set button style
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetStyle(
                                  MMI_CTRL_ID_T         ctrl_id,        //in
                                  GUIBUTTON_STYLE_E     style           //in:
                                  )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        button_ctrl_ptr->style = style;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set button release background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetBg(
                               MMI_CTRL_ID_T    ctrl_id,    //in
                               GUI_BG_T         *bg_ptr     //in:may PNULL
                               )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //set fg
        if (PNULL != bg_ptr)
        {
            button_ctrl_ptr->release.bg = *bg_ptr;

            //reset button sheen
            ResetButtonSheen(button_ctrl_ptr);

            //reset button pop
            ResetButtonPop(button_ctrl_ptr);        
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button release foreground
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetFg(
                               MMI_CTRL_ID_T    ctrl_id,    //in
                               GUI_BG_T         *fg_ptr     //in:may PNULL
                               )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //set fg
        if (PNULL != fg_ptr)
        {
            button_ctrl_ptr->release.fg = *fg_ptr;

            //reset button sheen
            ResetButtonSheen(button_ctrl_ptr);

            //reset button pop
            ResetButtonPop(button_ctrl_ptr);
        }
        
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button pressed background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetPressedBg(
                                      MMI_CTRL_ID_T    ctrl_id,    //in
                                      GUI_BG_T         *bg_ptr     //in:may PNULL
                                      )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //set fg
        if (PNULL != bg_ptr)
        {
            button_ctrl_ptr->pressed.bg = *bg_ptr;

            //reset button sheen
            ResetButtonSheen(button_ctrl_ptr);
            
            //reset button pop
            ResetButtonPop(button_ctrl_ptr);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button pressed foreground
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetPressedFg(
                                      MMI_CTRL_ID_T    ctrl_id,    //in
                                      GUI_BG_T         *fg_ptr     //in:may PNULL
                                      )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //set fg
        if (PNULL != fg_ptr)
        {
            button_ctrl_ptr->pressed.fg = *fg_ptr;

            //reset button sheen
            ResetButtonSheen(button_ctrl_ptr);

            //reset button pop
            ResetButtonPop(button_ctrl_ptr);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button gray background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetGrayed(
                                   MMI_CTRL_ID_T    ctrl_id,    //in:
                                   BOOLEAN          is_gray,    //in:
                                   GUI_BG_T         *gray_ptr,  //in:
                                   BOOLEAN          is_update   //in:
                                   )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //reset button sheen
        ResetButtonSheen(button_ctrl_ptr);
        
        //reset button pop
        ResetButtonPop(button_ctrl_ptr);

        //set gray
        button_ctrl_ptr->is_gray = is_gray;

        //set gray
        if (PNULL != gray_ptr)
        {
            button_ctrl_ptr->gray.bg = *gray_ptr;
        }
        
        //update
        if (is_update)
        {
            ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_NONE);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button gray foreground
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetGrayedFg(
                                     MMI_CTRL_ID_T      ctrl_id,    //in:
                                     BOOLEAN            is_gray,    //in:
                                     GUI_BG_T           *gray_ptr,  //in:
                                     BOOLEAN            is_update   //in:
                                     )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //reset button sheen
        ResetButtonSheen(button_ctrl_ptr);

        //reset button pop
        ResetButtonPop(button_ctrl_ptr);

        //set gray
        button_ctrl_ptr->is_gray = is_gray;

        //set gray
        if (PNULL != gray_ptr)
        {
            button_ctrl_ptr->gray.fg = *gray_ptr;
        }
        
        //update
        if (is_update)
        {
            ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_NONE);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button text by id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetTextId(
                                   MMI_CTRL_ID_T    ctrl_id,    //in:
                                   MMI_TEXT_ID_T    text_id     //in
                                   )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);

    if (PNULL != button_ctrl_ptr)
    {
        button_ctrl_ptr->text_info.text_id = text_id;
        
        button_ctrl_ptr->text_info.text_type = GUISTR_TEXT_ID;
        
        ButtonFreeText(button_ctrl_ptr);

        result = TRUE;
    }
    return (result);
}

/*****************************************************************************/
//  Description : set button text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetText(
                                 MMI_CTRL_ID_T  ctrl_id,    //in:
                                 wchar          *str_ptr,   //in:
                                 uint16         str_len     //in:
                                 )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //free string
        ButtonFreeText(button_ctrl_ptr);

        button_ctrl_ptr->text_info.text_type = GUISTR_TEXT_BUFFER;

        if (str_len > 0 && PNULL != str_ptr)
        {
            //alloc memory
            button_ctrl_ptr->text_info.str_info.wstr_ptr = SCI_ALLOC_APP((str_len + 1) * sizeof(wchar));
            SCI_MEMSET(button_ctrl_ptr->text_info.str_info.wstr_ptr,0,((str_len + 1) * sizeof(wchar)));
            
            //copy string
            MMI_MEMCPY(button_ctrl_ptr->text_info.str_info.wstr_ptr,(str_len * sizeof(wchar)),
                str_ptr,(str_len * sizeof(wchar)),
                (str_len * sizeof(wchar)));
            
            button_ctrl_ptr->text_info.str_info.wstr_len = str_len;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button text align
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetTextAlign(
                                      MMI_CTRL_ID_T     ctrl_id,//in:
                                      GUI_ALIGN_E       align   //in
                                      )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //set align
        button_ctrl_ptr->theme.text_align = align;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button font color
//  Parameter: [In] ctrl_id : button control id
//             [In] font_color : button font color
//             [Out] None
//             [Return] result : TRUE:secceed , FALSE : failed
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetFontColor(
                                      MMI_CTRL_ID_T     ctrl_id,//in:
                                      CAF_COLOR_T       font_color//in
                                      )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //set font color
        button_ctrl_ptr->theme.font.color = font_color;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button font color
//  Parameter: [In] ctrl_id : button control id
//             [In] font_size : button font size
//             [Out] None
//             [Return] result : TRUE:secceed , FALSE : failed
//  Author: qingjun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetFontSize(MMI_CTRL_ID_T ctrl_id, // in:
                                      CAF_FONT_TYPE_E font_size // in
)
{
    BOOLEAN result = FALSE;
    CTRLBUTTON_OBJ_T *button_ctrl_ptr = PNULL;

    // get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        // set font color
        button_ctrl_ptr->theme.font.font = font_size;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button callback function
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetCallBackFunc(
                                         MMI_CTRL_ID_T              ctrl_id,
                                         GUIBUTTON_CALLBACK_FUNC    func
                                         )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //set callback
        button_ctrl_ptr->callback_func = func;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button callback function
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetCallBackFuncExt(
                                            MMI_CTRL_ID_T               ctrl_id,
                                            GUIBUTTON_CALLBACK_FUNC_EXT func
                                            )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //set callback
        button_ctrl_ptr->callback_func_ext = func;

        result = TRUE;
    }

    return (result);
}
/*****************************************************************************/
//  Description : set button is handle long msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetHandleLong(
                                       MMI_CTRL_ID_T    ctrl_id,
                                       BOOLEAN          is_handle
                                       )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //set is handle long
        button_ctrl_ptr->is_handle_long = is_handle;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button is transparent display image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetTransparent(
                                        MMI_CTRL_ID_T   ctrl_id,
                                        BOOLEAN         is_transparent
                                        )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //set is transparent
        button_ctrl_ptr->is_transparent = is_transparent;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description :set button run sheen
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetRunSheen(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     BOOLEAN        is_runsheen
                                     )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;
    
    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if(PNULL != button_ctrl_ptr)
    {
        if (is_runsheen)
        {
            button_ctrl_ptr->theme.anim_type = GUIBUTTON_RUNSHEEN_ANIM;
        }
        else
        {
            button_ctrl_ptr->theme.anim_type = GUIBUTTON_NONE_ANIM;
        }    
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : set button rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetRect(
                                MMI_CTRL_ID_T   ctrl_id,    //in:
                                GUI_RECT_T      *rect_ptr   //in:
                                )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        result =  ButtonCtrlSetRect((CTRLBASE_OBJ_T*)button_ctrl_ptr,rect_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button is permit display border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_PermitBorder(
                                      MMI_CTRL_ID_T     ctrl_id,
                                      BOOLEAN           is_permit
                                      )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        button_ctrl_ptr->is_permit_border = is_permit;
        
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button font
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetFont(
                                 MMI_CTRL_ID_T      ctrl_id,    //in:
                                 GUI_FONT_ALL_T     *font_ptr   //in:
                                 )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        result =  ButtonCtrlSetFont((CTRLBASE_OBJ_T*)button_ctrl_ptr,font_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button is visible
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetVisible(
                                    MMI_CTRL_ID_T   ctrl_id,
                                    BOOLEAN         is_visible,
                                    BOOLEAN         is_update
                                    )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;
    CTRLBASE_OBJ_T          *base_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //set visible
        VTLBASE_SetVisible((CTRLBASE_OBJ_T*)button_ctrl_ptr, is_visible);

        if (is_update)
        {
            if (is_visible)
            {
                //update display
                ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_NONE);
            }
            else
            {
                base_ctrl_ptr = (CTRLBASE_OBJ_T*)button_ctrl_ptr;

                //notify parent update bg
                CTRLMSG_SendNotifyEx(base_ctrl_ptr->handle,MSG_NOTIFY_UPDATE,&base_ctrl_ptr->rect);
            }
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set button owner draw callback function
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBUTTON_SetOwnerDraw(
                                   MMI_CTRL_ID_T                ctrl_id,
                                   GUIBUTTON_OWNER_DRAW_FUNC    func
                                   )
{
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //set function
        button_ctrl_ptr->OwnerDrawFunc = func;
    }
}

/*****************************************************************************/
//  Description : update button
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_Update(
                                MMI_CTRL_ID_T   ctrl_id
                                )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //update display
        ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_NONE);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : update button text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_UpdateText(
                                    MMI_CTRL_ID_T   ctrl_id
                                    )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //display text
        ButtonDrawText(button_ctrl_ptr);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : forbid paint or not
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBUTTON_SetForbidPaint(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     BOOLEAN        is_forbid_paint
                                     )
{
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        button_ctrl_ptr->is_forbid_paint = is_forbid_paint;
    }
}

/*****************************************************************************/
//  Description : set button Border
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetBorder(
                                   MMI_CTRL_ID_T    ctrl_id,    //in:
                                   GUI_BORDER_T     *border_ptr,
                                   BOOLEAN          is_update
                                   )
{
    BOOLEAN                 result = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    //get button pointer by control id
    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL != button_ctrl_ptr)
    {
        //reset button sheen
         ButtonCtrlSetBorder((CTRLBASE_OBJ_T*)button_ctrl_ptr,border_ptr);

        //update
        if (is_update)
        {
            ButtonDraw(button_ctrl_ptr, GUIBUTTON_OWNDRAW_NONE);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Set Pop Anim Info
//  Global resource dependence : 
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBUTTON_SetPopAnimInfo(
                                        MMI_CTRL_ID_T ctrl_id,
                                        int32*        total_frame_ptr,    
                                        int32*        radius_ptr,         
                                        uint32*       filled_color_ptr 
                                        )
{
    BOOLEAN                 result           = FALSE;
    CTRLBUTTON_OBJ_T        *button_ctrl_ptr = PNULL;

    button_ctrl_ptr = GetButtonPtr(ctrl_id);
    if (PNULL == button_ctrl_ptr)
    {
        return result;
    }
    
    button_ctrl_ptr->theme.anim_type = GUIBUTTON_POPBLINK_ANIM;

    if ((PNULL == total_frame_ptr)&&
        (PNULL == radius_ptr)&&
        (PNULL == filled_color_ptr))
    {
        result = TRUE;
        return result;
    }
        
    if (PNULL != total_frame_ptr)
    {
        button_ctrl_ptr->theme.pop_info.total_frame = *total_frame_ptr;
    }

    if (PNULL != radius_ptr)
    {
        button_ctrl_ptr->theme.pop_info.radius = *radius_ptr;
        button_ctrl_ptr->theme.pop_info.is_default_radius = FALSE;
    }

    if (PNULL != filled_color_ptr)
    {
        button_ctrl_ptr->theme.pop_info.filled_color = *filled_color_ptr;
    }

    result = TRUE;
       
    return result;
}

/*Edit by script, ignore 4 case. Thu Apr 26 19:01:13 2012*/ //IGNORE9527

