/*****************************************************************************
** File Name:      guisoftkey_button.c                                       *
** Author:                                                                   *
** Date:           06/20/2011                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME             DESCRIPTION                             *
** 06/20/2011       jibin              Create                                *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_gui_trc.h"
#include "guisoftkey.h"
#include "ctrlsoftkey.h"
#include "guilcd.h"
#include "mmi_theme.h"
#include "guifont.h"
#include "mmi_textfun.h"
#include "mmitheme_pubwin.h"
#include "mmk_app.h"
#include "guires.h"
#include "guistring.h"
#include "mmk_window_internal.h"
#include "mmitheme_softkey.h"
#include "guictrl_api.h"
#include "guibutton.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : get button handle
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetButtonHandle(
                                   MMI_HANDLE_T win_handle,
                                   uint32 index
                                   );

/*****************************************************************************/
//  Description :根据当前文字设置获得当前匹配的风格
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL GUISOFTKEY_BUTTONSTYLE_E GetStyleFromHandle(
                                                  CTRLSOFTKEY_OBJ_T		*softkey_ptr
                                                  );

/*****************************************************************************/
// 	Description : 设置softkey button的文本ID,并告知控件是否立即刷新
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetButtonTextIdOrPtr(
                                   CTRLSOFTKEY_OBJ_T    *softkey_ptr,
                                   MMI_STRING_T     *str_info_ptr,   // the softkey id
                                   MMI_TEXT_ID_T    text_id,   // the softkey id
                                   uint16           index,
                                   BOOLEAN          is_need_update       // whether update
                                   );

/*****************************************************************************/
//  Description : 调整button ctrl rect
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void AdjustButtonCtrlRect(
                                CTRLSOFTKEY_OBJ_T* softkey_ptr
                                );

/*****************************************************************************/
//  Description : set button
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SetButton(
                     CTRLSOFTKEY_OBJ_T* softkey_ptr,
                     MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                     MMI_TEXT_ID_T   middlesoft_id, // the middle softkey id
                     MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                     BOOLEAN         is_need_update       // whether update
                     );

/*****************************************************************************/
//  Description : set button rect
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SetButtonRect(
                         MMI_CTRL_ID_T   ctrl_id,    //in:
                         GUI_RECT_T      *rect_ptr   //in:
                         );
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
// Description : 初始化button ctrl
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL BOOLEAN InitButtonCtrl(
                             CTRLSOFTKEY_OBJ_T		*softkey_ptr,
                             GUIBUTTONSOFTKEY_INIT_DATA_T *init_data_ptr
                             )
{
    MMI_BUTTONSOFTKEY_STYLE_T buttonsoftkey_style = {0};
    
    if (PNULL == init_data_ptr)
    {
        //SCI_TRACE_LOW:"InitButtonCtrl() invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_BUTTON_124_112_2_18_3_21_31_323,(uint8*)"");
        return FALSE;
    }

    if (!MMITHEME_GetButtonSoftkeyStyle(&buttonsoftkey_style))
    {
        return FALSE;
    }

    softkey_ptr->bg_image = buttonsoftkey_style.bg_image;

    SetButton(softkey_ptr, init_data_ptr->text_id_arr[0], init_data_ptr->text_id_arr[1], init_data_ptr->text_id_arr[2], TRUE);

    return TRUE;
}

/*****************************************************************************/
//  Description :创建button softkey
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_Init(
                                     CTRLSOFTKEY_OBJ_T		*softkey_ptr,
                                     GUIBUTTONSOFTKEY_INIT_DATA_T *init_data_ptr
                                     )
{   
    if ((PNULL == softkey_ptr)||(PNULL == init_data_ptr))
    {
        //SCI_TRACE_LOW:"GUIBUTTONSOFTKEY_Init() invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_BUTTON_151_112_2_18_3_21_31_324,(uint8*)"");
        return FALSE;
    }

    softkey_ptr->is_visible = TRUE;

    //增加softkey要调整client rect
    MMK_SetWinSoftkeyCtrlId(init_data_ptr->win_handle, init_data_ptr->handle);

    InitButtonCtrl(softkey_ptr, init_data_ptr);    
    
    return TRUE;
}

/*****************************************************************************/
// Description : get button handle
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLBUTTONSOFTKEY_GetButtonHandle(
                                                     MMI_HANDLE_T win_handle,
                                                     uint32 index
                                                     )
{
    return GetButtonHandle(win_handle, index);
}

/*****************************************************************************/
// Description : get button handle
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetButtonHandle(
                                   MMI_HANDLE_T win_handle,
                                   uint32 index
                                   )
{
    MMI_HANDLE_T button_handle = 0;
    MMI_CTRL_ID_T ctrl_id_arr[3] = {0};
    
    MMITHEME_GetSoftkeyButtonCtrlId(ctrl_id_arr);
    button_handle = MMK_GetCtrlHandleByWin( win_handle, ctrl_id_arr[index] );
    
    return button_handle;
}

/*****************************************************************************/
// Description : 画softkey
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void GUIBUTTONOFTKEY_Update(
                                   CTRLSOFTKEY_OBJ_T* softkey_ptr
                                   )
{
    uint32 i = 0;    
    CTRLBASE_OBJ_T * ctrl_ptr = (CTRLBASE_OBJ_T *)softkey_ptr;

    if (PNULL == softkey_ptr)
    {
        //SCI_TRACE_LOW:"GUIBUTTONOFTKEY_Update() invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_BUTTON_207_112_2_18_3_21_31_325,(uint8*)"");
        return;
    }
    GUIBUTTONSOFTKEY_DrawBg(softkey_ptr,&ctrl_ptr->rect);
    for (i = 0; i < GUISOFTKEY_BUTTON_NUM; i++)
    {
        GUIBUTTON_Update(GetButtonHandle(softkey_ptr->win_handle,i));
    }
}

/*****************************************************************************/
//  Description : 设置是否可见
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetVisible(
                                           CTRLSOFTKEY_OBJ_T* softkey_ptr,
                                           BOOLEAN         is_visible
                                           )
{
    uint32 i = 0;
    
    if (PNULL == softkey_ptr)
    {
        //SCI_TRACE_LOW:"GUIBUTTONSOFTKEY_SetVisible() invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_BUTTON_230_112_2_18_3_21_31_326,(uint8*)"");
        return FALSE;
    }
    for (i = 0; i < GUISOFTKEY_BUTTON_NUM; i++)
    {
        GUIBUTTON_SetVisible(GetButtonHandle(softkey_ptr->win_handle,i), is_visible, FALSE);
    }
    return TRUE;
}

/*****************************************************************************/
// Description : set  the valid state of the softkey bar button
// Global resource dependence : 
// Author:Robert Lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  GUIBUTTONSOFTKEY_SetButtonState(
                                                CTRLSOFTKEY_OBJ_T* softkey_ptr,
                                                GUISOFTKEY_INDEX button_index,
                                                BOOLEAN         is_gray,
                                                BOOLEAN		 is_long_press
                                                )
{
    if (PNULL == softkey_ptr)
    {
        //SCI_TRACE_LOW:"GUIBUTTONSOFTKEY_SetButtonState() invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_BUTTON_253_112_2_18_3_21_31_327,(uint8*)"");
        return FALSE;
    }
    GUIBUTTON_SetGrayed(GetButtonHandle(softkey_ptr->win_handle,button_index), is_gray, PNULL, FALSE);
    GUIBUTTON_SetHandleLong(GetButtonHandle(softkey_ptr->win_handle,button_index), is_long_press);
    
    return TRUE;
}

/*****************************************************************************/
// Description : set  the valid state of the softkey bar button
// Global resource dependence : 
// Author:Robert Lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  GUIBUTTONSOFTKEY_SetButtonGray(
                                                CTRLSOFTKEY_OBJ_T* softkey_ptr,
                                                GUISOFTKEY_INDEX button_index,
                                                BOOLEAN         is_gray
                                                )
{
    if (PNULL == softkey_ptr)
    {
        //SCI_TRACE_LOW:"GUIBUTTONSOFTKEY_SetButtonState() invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_BUTTON_274_112_2_18_3_21_32_328,(uint8*)"");
        return FALSE;
    }

    GUIBUTTON_SetGrayed(GetButtonHandle(softkey_ptr->win_handle,button_index), is_gray, PNULL, TRUE);
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : 设置softkey的文本ID,并告知控件是否立即刷新
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetTextId(
                                          CTRLSOFTKEY_OBJ_T* softkey_ptr,
                                          MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                          MMI_TEXT_ID_T   middlesoft_id, // the middle softkey id
                                          MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                                          BOOLEAN         is_need_update       // whether update
                                          )
{
    if (PNULL == softkey_ptr)
    {
        //SCI_TRACE_LOW:"GUIBUTTONSOFTKEY_SetTextId() invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISOFTKEY_BUTTON_297_112_2_18_3_21_32_329,(uint8*)"");
        return FALSE;
    }

    SetButton(softkey_ptr, leftsoft_id, middlesoft_id, rightsoft_id, is_need_update);

    return TRUE;
}

/*****************************************************************************/
// Description : 画softkey 背景
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void GUIBUTTONSOFTKEY_DrawBg(
                                    CTRLSOFTKEY_OBJ_T   *softkey_ptr,
                                    GUI_RECT_T      *rect_ptr
                                    )
{
    GUI_BG_T            bg_info = {0};
    GUI_RECT_T          img_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T *    ctrl_ptr = (CTRLBASE_OBJ_T *)softkey_ptr;

    //set bg
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id  = softkey_ptr->bg_image;

    //set image rect
    img_rect.left   = (int16)(rect_ptr->left - ctrl_ptr->rect.left);
    img_rect.top    = (int16)(rect_ptr->top - ctrl_ptr->rect.top);
    img_rect.right  = (int16)(img_rect.left + rect_ptr->right - rect_ptr->left);
    img_rect.bottom = (int16)(img_rect.top + rect_ptr->bottom - rect_ptr->top);

    //set bg display info
    bg_display.is_transparent = TRUE;
    bg_display.rect           = *rect_ptr;
    bg_display.display_rect   = *rect_ptr;
    bg_display.img_rect       = img_rect;
    bg_display.ctrl_handle    = ctrl_ptr->handle;
    bg_display.win_handle     = softkey_ptr->win_handle;

    //display bg
    GUI_DisplayBg(&bg_info,
        &bg_display,
        &ctrl_ptr->lcd_dev_info);
}

/*****************************************************************************/
// 	Description : 设置softkey button的文本ID,并告知控件是否立即刷新
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetButtonTextId(
                                                CTRLSOFTKEY_OBJ_T*  softkey_ptr,
                                                MMI_TEXT_ID_T   text_id,   // the left softkey id
                                                GUISOFTKEY_INDEX button_num,
                                                BOOLEAN         is_need_update       // whether update
                                                )
{
    SetButtonTextIdOrPtr(softkey_ptr, PNULL, text_id, button_num, is_need_update);
    AdjustButtonCtrlRect(softkey_ptr);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 根据当前文字设置调整button ctrl rect
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void AdjustButtonCtrlRect(
                                CTRLSOFTKEY_OBJ_T* softkey_ptr
                                )
{
    uint32           i = 0;
    uint32           valid_index = 0;
    MMI_HANDLE_T     button_handle = 0;
    GUI_RECT_T       button_rect_arr[GUISOFTKEY_BUTTON_NUM] = {0};
    CTRLBASE_OBJ_T * ctrl_ptr = (CTRLBASE_OBJ_T *) softkey_ptr;

    if (PNULL == softkey_ptr)
    {
        return;
    }
    if (MMITHEME_CalcSoftkeyButtonRect(&(ctrl_ptr->rect), GetStyleFromHandle(softkey_ptr), button_rect_arr))
    {
        for ( i = 0; i < GUISOFTKEY_BUTTON_NUM; i++ )
        {
            button_handle = GetButtonHandle(softkey_ptr->win_handle, i);  
            
            if (0 != button_handle)
            {
                SetButtonRect(button_handle, &button_rect_arr[valid_index++]);
                //            GUIAPICTRL_SetBothRect( button_handle, &buttonsoftkey_style.button_both_rect[valid_index++] );
            }
        }
    }
}

/*****************************************************************************/
// 	Description : 设置softkey button的文本ID,并告知控件是否立即刷新
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetButtonTextPtr(
                                                 CTRLSOFTKEY_OBJ_T* softkey_ptr,
                                                 MMI_STRING_T    str_info,   // the left softkey id
                                                 GUISOFTKEY_INDEX button_num,
                                                 BOOLEAN         is_need_update       // whether update
                                                 )
{
    SetButtonTextIdOrPtr(softkey_ptr, &str_info, 0, button_num, is_need_update);
    AdjustButtonCtrlRect(softkey_ptr);
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : 设置softkey button的文本ID,并告知控件是否立即刷新
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SetButtonTextIdOrPtr(
                                   CTRLSOFTKEY_OBJ_T    *softkey_ptr,
                                   MMI_STRING_T     *str_info_ptr,   // the softkey id
                                   MMI_TEXT_ID_T    text_id,   // the softkey id
                                   uint16           index,
                                   BOOLEAN          is_need_update       // whether update
                                   )
{
    MMI_HANDLE_T button_handle = 0;
    MMI_CTRL_ID_T ctrl_id_arr[3] = {0};
    MMI_BUTTONSOFTKEY_STYLE_T buttonsoftkey_style = {0};
    
    if ((PNULL == softkey_ptr) || index >= GUISOFTKEY_BUTTON_NUM)
    {
        return FALSE;
    }
    //get style
    if (!MMITHEME_GetButtonSoftkeyStyle(&buttonsoftkey_style))
    {
        return FALSE;
    }
    //get ctrl_id
    MMITHEME_GetSoftkeyButtonCtrlId(ctrl_id_arr);
    //get handle
    button_handle = GetButtonHandle(softkey_ptr->win_handle, index);  
    if ((PNULL == str_info_ptr)
        && MMI_IsTextNull(text_id))
    {
        MMK_DestroyControl( button_handle );
    }           
    else
    {
        if ( 0 == button_handle )
        {
            GUIBUTTON_INIT_DATA_T init_data = {0};
            
            //init
            init_data.bg.bg_type = GUI_BG_IMG;
            init_data.bg.img_id  = buttonsoftkey_style.img_id; //win_info_ptr->pubwin_theme.button_img_id;
            button_handle = GUIBUTTON_CreateDynamic( softkey_ptr->win_handle, ctrl_id_arr[index], &init_data );
            
            GUIBUTTON_SetFont( button_handle, &buttonsoftkey_style.font );
            
            init_data.bg.bg_type = GUI_BG_IMG;
            init_data.bg.img_id = buttonsoftkey_style.grayed_img_id;
            GUIBUTTON_SetGrayed(button_handle, FALSE, &init_data.bg, FALSE);
            
            GUIBUTTON_SetRunSheen( button_handle, FALSE );
            
            init_data.bg.img_id = buttonsoftkey_style.pressed_img_id;
            GUIBUTTON_SetPressedBg( button_handle, &init_data.bg );
            
            GUIAPICTRL_SetState(button_handle, GUICTRL_STATE_TOPMOST, TRUE);
            GUIAPICTRL_SetState(button_handle, GUICTRL_STATE_KEYMODE_ACTIVE, TRUE);
            //            MMK_SetAtvCtrl(softkey_ptr->win_handle, button_handle);
        }
        
        if (PNULL == str_info_ptr)
        {
            GUIBUTTON_SetTextId( button_handle, text_id );
        }
        else
        {
            GUIBUTTON_SetText(button_handle, str_info_ptr->wstr_ptr, str_info_ptr->wstr_len);
        }    
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description :set state
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetState(
                                         CTRLSOFTKEY_OBJ_T		*softkey_ptr,
                                         GUICTRL_STATE_T  state,
                                         BOOLEAN          is_true
                                         )
{
    uint32 i = 0;
    
    for (i=0; i<GUISOFTKEY_BUTTON_NUM;i++)
    {
        GUIAPICTRL_SetState(GetButtonHandle(softkey_ptr->win_handle,i),state,is_true);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description :根据当前文字设置获得当前匹配的风格
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL GUISOFTKEY_BUTTONSTYLE_E GetStyleFromHandle(
                                                  CTRLSOFTKEY_OBJ_T		*softkey_ptr
                                                  )
{
    uint32 i = 0;
    GUISOFTKEY_BUTTONSTYLE_E style = GUISOFTKEY_BUTTONSTYLE_NONE;
    uint32 button_num = 0;
    MMI_HANDLE_T button_handle = 0;
    
    if (PNULL != softkey_ptr)
    {
        for (i = 0; i < GUISOFTKEY_BUTTON_NUM; i++)
        {
            button_handle = GetButtonHandle(softkey_ptr->win_handle,i);
            if (0 != button_handle)
            {
                button_num++;
            }
        }
        style = (GUISOFTKEY_BUTTONSTYLE_NONE+button_num);
    }
    return style;
}

/*****************************************************************************/
//  Description :set rect
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTONSOFTKEY_SetRect(
                                        CTRLSOFTKEY_OBJ_T		*softkey_ptr,
                                        const CAF_RECT_T  *rect_ptr
                                        )
{
    GUI_RECT_T button_rect_arr[GUISOFTKEY_BUTTON_NUM] = {0};
    MMI_HANDLE_T button_handle = 0;
    uint32 i = 0;
    uint32 valid_index = 0;
    
    if ((PNULL == softkey_ptr) || (PNULL == rect_ptr))
    {
        return FALSE;
    }
    
    if (GUI_IsRectEmpty(*rect_ptr))
    {
        SetButtonRect(GetButtonHandle(softkey_ptr->win_handle,0), &button_rect_arr[0]);
        SetButtonRect(GetButtonHandle(softkey_ptr->win_handle,1), &button_rect_arr[1]);
        SetButtonRect(GetButtonHandle(softkey_ptr->win_handle,2), &button_rect_arr[2]);
        return TRUE;
    }
    else
    {        
        if (MMITHEME_CalcSoftkeyButtonRect(rect_ptr, GetStyleFromHandle(softkey_ptr), button_rect_arr))
        {
            for (i = 0; i < GUISOFTKEY_BUTTON_NUM; i++)
            {
                button_handle = GetButtonHandle(softkey_ptr->win_handle,i);
                if (0 != button_handle)
                {
                    SetButtonRect(button_handle, &button_rect_arr[valid_index++]);
                }
            }
            return TRUE;
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description : set button
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void SetButton(
                     CTRLSOFTKEY_OBJ_T* softkey_ptr,
                     MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                     MMI_TEXT_ID_T   middlesoft_id, // the middle softkey id
                     MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                     BOOLEAN         is_need_update       // whether update
                     )
{
    
    MMI_TEXT_ID_T  softkey_id[GUISOFTKEY_BUTTON_NUM] = {0};
    int32          i = 0;
    
    if ( PNULL != softkey_ptr )
    {      
        softkey_id[0] = leftsoft_id;
        softkey_id[1] = middlesoft_id;
        softkey_id[2] = rightsoft_id;
        
        for ( i = 0; i < GUISOFTKEY_BUTTON_NUM; i++ )
        {
            SetButtonTextIdOrPtr(softkey_ptr, PNULL, softkey_id[i], i,FALSE);
        }
        AdjustButtonCtrlRect(softkey_ptr);
    }
}

/*****************************************************************************/
//  Description : set button rect
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SetButtonRect(
                         MMI_CTRL_ID_T   ctrl_id,    //in:
                         GUI_RECT_T      *rect_ptr   //in:
                         )
{
    if (GUI_IsInvalidRect(*rect_ptr))
    {
        GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_KEYMODE_ACTIVE, FALSE);
    }
    else
    {
        GUIAPICTRL_SetState(ctrl_id, GUICTRL_STATE_KEYMODE_ACTIVE, TRUE);
    }

    GUIBUTTON_SetRect(ctrl_id, rect_ptr);
}

/*****************************************************************************/
//  Description : get style from text
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC GUISOFTKEY_BUTTONSTYLE_E CTRLBUTTONSOFTKEY_GetStyleFromText( 
                                                                  MMI_TEXT_ID_T		leftsoft_id,
                                                                  MMI_TEXT_ID_T		midsoft_id,
                                                                  MMI_TEXT_ID_T		rightsoft_id
                                                                  )
{
    GUISOFTKEY_BUTTONSTYLE_E style = GUISOFTKEY_BUTTONSTYLE_NONE;
    uint32 num = 0;
    
    if (!MMI_IsTextNull(leftsoft_id))
    {
        num++;
    }
    if (!MMI_IsTextNull(midsoft_id))
    {
        num++;
    }
    if (!MMI_IsTextNull(rightsoft_id))
    {
        num++;
    }
    switch (num)
    {
    case 1:
        style = GUISOFTKEY_BUTTONSTYLE_1BUTTON;
        break;
        
    case 2:
        style = GUISOFTKEY_BUTTONSTYLE_2BUTTON;
        break;
        
    case 3:
        style = GUISOFTKEY_BUTTONSTYLE_3BUTTON;
        break;
        
    case 0:
    default:
        style = GUISOFTKEY_BUTTONSTYLE_NONE;
        break;
    }
    return style;
}


// end file
