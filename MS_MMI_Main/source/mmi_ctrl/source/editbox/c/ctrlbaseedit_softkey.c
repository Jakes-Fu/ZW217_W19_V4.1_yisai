/*****************************************************************************
** File Name:      ctrlbaseedit_softkey.c                                    *
** Author:         hua.fang                                                  *
** Date:           09/10/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2012        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "ctrlbaseedit.h"
#include "mmi_gui_trc.h"
#include "guiwin.h"
#include "mmi_theme.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
extern void MMIPUB_GetWinSoftkeyGray (
    MMI_WIN_ID_T    win_id,
    BOOLEAN         *is_left_gray_ptr,
    BOOLEAN         *is_middle_gray_ptr,
    BOOLEAN         *is_right_gray_ptr
);  /*lint -esym(526,MMIPUB_GetWinSoftkeyGray) */
#endif  // MMIPUBWIN_PDASTYLE_SUPPORT

//xiyuan edit for pclint
#ifdef GUIF_SOFTKEY_PDASTYLE
extern BOOLEAN MMIRES_IsTextNull (MMI_TEXT_ID_T text_id);   /*lint -esym(526,MMIRES_IsTextNull) */
#endif

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : softkey is valid
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsValidSoftkey (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr,
    uint16                 softkey_index
);

/*****************************************************************************/
//  Description : get default softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetDefaultSoftkey (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
);

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
/*****************************************************************************/
//  Description : 判断button是否灰化
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSoftkeyButtonGray (
    MMI_TEXT_ID_T     text_id
);

/*****************************************************************************/
//  Description : 获取softkey的内容
//  Global resource dependence :
//  Author: Jassmine
//  Note:
// 为保证一定能显示内容，这里判断下两个text id，获取有效的值
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetSoftkeyButtonText (
    MMI_TEXT_ID_T  cur_text_id,
    MMI_TEXT_ID_T  text_id_1,
    MMI_TEXT_ID_T  text_id_2
);
#endif

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : 让softkey处理消息
// Global resource dependence :
// Author:liqing.peng
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E BASEEDIT_HandleSoftkeyMsg (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr,
    MMI_MESSAGE_ID_E       msg_id,
    DPARAM                 param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    switch (msg_id)
    {
    case MSG_APP_OK:
        if (IsValidSoftkey (baseedit_ctrl_ptr, 0))
        {
            //notify parent control
            result = BASEEDIT_NotifyParentMsg (base_ctrl_ptr->handle, MSG_NOTIFY_OK);
        }
        break;

    case MSG_APP_WEB:
        if ((IsValidSoftkey (baseedit_ctrl_ptr, 0)) ||
            (IsValidSoftkey (baseedit_ctrl_ptr, 1)))
        {
            //notify parent control
            result = BASEEDIT_NotifyParentMsg (base_ctrl_ptr->handle, MSG_NOTIFY_MIDSK);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
// Description : 设置softkey的状态
// Global resource dependence :
// Author:liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_SetSoftkeyBtnState (
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr,
    uint16                  button_index,
    BOOLEAN                 is_gray,
    BOOLEAN		            is_long_press
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

#if defined(MMIPUBWIN_PDASTYLE_SUPPORT) && !defined(MMI_PDA_SUPPORT)
    // 此风格下，让pubwin的button处理长按键
    if (MMK_GetWinDisplayStyleState (base_ctrl_ptr->win_handle, WS_HAS_PROMPT_WIN))
    {
        extern void MMIPUB_SetWinSoftkeyHandleLong (
            MMI_WIN_ID_T     win_id,
            uint16           button_index,
            BOOLEAN          is_handle_long
        );/*lint -esym(526,MMIPUB_SetWinSoftkeyHandleLong) */

        MMIPUB_SetWinSoftkeyHandleLong (base_ctrl_ptr->win_handle, button_index, is_long_press);

        result = TRUE;
    }
    else
#endif
    {
        result = GUIWIN_SetSoftkeyBtnState (
                     base_ctrl_ptr->win_handle,
                     button_index, is_gray, is_long_press);
    }

    return result;
}

/*****************************************************************************/
//  Description : display edit softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_DisplaySoftkey (
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr
)
{
    BOOLEAN         result = FALSE;
    BOOLEAN         is_softkey_set = FALSE;
    MMI_TEXT_ID_T   left_softkey_id = 0;
    MMI_TEXT_ID_T   mid_softkey_id = 0;
    MMI_TEXT_ID_T   right_softkey_id = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    if (base_ctrl_ptr->is_active)
    {
        //get default softkey
        GetDefaultSoftkey (baseedit_ctrl_ptr);

        //set softkey
        left_softkey_id  = baseedit_ctrl_ptr->common_theme.softkey.button[0].text_1;
        mid_softkey_id   = baseedit_ctrl_ptr->common_theme.softkey.button[1].text_1;
        right_softkey_id = baseedit_ctrl_ptr->common_theme.softkey.button[2].text_1;

        //get left softkey
        if ((PNULL != baseedit_ctrl_ptr->common_theme.softkey.button[0].setsoftkey) &&
            (baseedit_ctrl_ptr->common_theme.softkey.button[0].setsoftkey (baseedit_ctrl_ptr->str_ptr, baseedit_ctrl_ptr->str_len, &left_softkey_id)))
        {
            result = TRUE;
        }

        if ((!result) &&
            (0 != baseedit_ctrl_ptr->common_theme.softkey.button[0].str_len) &&
            (baseedit_ctrl_ptr->str_len >= baseedit_ctrl_ptr->common_theme.softkey.button[0].str_len))
        {
            left_softkey_id = baseedit_ctrl_ptr->common_theme.softkey.button[0].text_2;
        }

        result = FALSE;

        //get middle softkey
        if ((PNULL != baseedit_ctrl_ptr->common_theme.softkey.button[1].setsoftkey) &&
            (baseedit_ctrl_ptr->common_theme.softkey.button[1].setsoftkey (baseedit_ctrl_ptr->str_ptr, baseedit_ctrl_ptr->str_len, &mid_softkey_id)))
        {
            result = TRUE;
        }

        if ((!result) &&
            (0 != baseedit_ctrl_ptr->common_theme.softkey.button[1].str_len) &&
            (baseedit_ctrl_ptr->str_len >= baseedit_ctrl_ptr->common_theme.softkey.button[1].str_len))
        {
            mid_softkey_id = baseedit_ctrl_ptr->common_theme.softkey.button[1].text_2;
        }

        result = FALSE;

        //display right softkey
        if ((PNULL != baseedit_ctrl_ptr->common_theme.softkey.button[2].setsoftkey) &&
            (baseedit_ctrl_ptr->common_theme.softkey.button[2].setsoftkey (baseedit_ctrl_ptr->str_ptr, baseedit_ctrl_ptr->str_len, &right_softkey_id)))
        {
            result = TRUE;
        }

        if ((!result) &&
            (!BASEEDIT_IsFixedFormat (baseedit_ctrl_ptr)) &&
            (baseedit_ctrl_ptr->common_theme.is_cancel_delete) &&
            (0 != baseedit_ctrl_ptr->common_theme.softkey.button[2].str_len) &&
            (baseedit_ctrl_ptr->str_len >= baseedit_ctrl_ptr->common_theme.softkey.button[2].str_len))
        {
            right_softkey_id = baseedit_ctrl_ptr->common_theme.softkey.button[2].text_2;
        }

// edit负责pubedit下softkey的显示，其内容都有edit来设置，所以应用要处理好显示内容
// 如果不想显示某个键，需要调用GUIEDIT_SetSoftkey将值清掉
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
        // 由于PUBWIN是PDA风格的话，softkey是由PUBWIN创建的BUTTON，所以设置
        // softkey就应该调用pubwin的接口
        if (MMK_GetWinDisplayStyleState (base_ctrl_ptr->win_handle, WS_HAS_PROMPT_WIN))
        {
            extern void MMIPUB_SetWinSoftkeyGray (
                MMI_WIN_ID_T    win_id,
                BOOLEAN         is_left_gray,
                BOOLEAN         is_middle_gray,
                BOOLEAN         is_right_gray,
                BOOLEAN         is_fresh
            );/*lint -esym(526,MMIPUB_SetWinSoftkeyGray) */

            extern void MMIPUB_SetWinSoftkeyEx (
                MMI_WIN_ID_T          win_id,
                MMI_TEXT_ID_T         left_sk_id,
                MMI_TEXT_ID_T         middle_sk_id,
                MMI_TEXT_ID_T         right_sk_id,
                BOOLEAN               is_fresh
            );/*lint -esym(526,MMIPUB_SetWinSoftkeyEx) */

#ifndef MMI_PDA_SUPPORT
            // 只有非PDA支持长按删除全部字串的功能
            extern void MMIPUB_SetWinSoftkeyHandleLong (
                MMI_WIN_ID_T     win_id,
                uint16           button_index,
                BOOLEAN          is_handle_long
            );/*lint -esym(526,MMIPUB_SetWinSoftkeyHandleLong) */

            // 设置右软键是否处理长按
            MMIPUB_SetWinSoftkeyHandleLong (
                base_ctrl_ptr->win_handle, 2,
                baseedit_ctrl_ptr->str_len > 0 ? TRUE : FALSE);

#endif  // MMI_PDA_SUPPORT

            // 设置内容
            MMIPUB_SetWinSoftkeyEx (
                base_ctrl_ptr->win_handle,
                GetSoftkeyButtonText (left_softkey_id,
                                      baseedit_ctrl_ptr->common_theme.softkey.button[0].text_1,
                                      baseedit_ctrl_ptr->common_theme.softkey.button[0].text_2),
                GetSoftkeyButtonText (mid_softkey_id,
                                      baseedit_ctrl_ptr->common_theme.softkey.button[1].text_1,
                                      baseedit_ctrl_ptr->common_theme.softkey.button[1].text_2),
                GetSoftkeyButtonText (right_softkey_id,
                                      baseedit_ctrl_ptr->common_theme.softkey.button[2].text_1,
                                      baseedit_ctrl_ptr->common_theme.softkey.button[2].text_2),
                TRUE);

            MMIPUB_SetWinSoftkeyGray (
                base_ctrl_ptr->win_handle,
                IsSoftkeyButtonGray (left_softkey_id),
                IsSoftkeyButtonGray (mid_softkey_id),
                IsSoftkeyButtonGray (right_softkey_id),
                TRUE);

            is_softkey_set = TRUE;
        }
#endif

#ifndef GUIF_SOFTKEY_PDASTYLE
        //display softkey
        // PDA风格，softkey不由edit控制
        if (!is_softkey_set)    /*lint !e774*/
        {
#if defined(SCRIPTEASY_SUPPORT)
            BOOLEAN need_update_left = (EDIT_BUTTON_TEXT_ID == baseedit_ctrl_ptr->common_theme.softkey.button[0].button_type);
            BOOLEAN need_update_mid = (EDIT_BUTTON_TEXT_ID == baseedit_ctrl_ptr->common_theme.softkey.button[1].button_type);
            BOOLEAN need_update_right = (EDIT_BUTTON_TEXT_ID == baseedit_ctrl_ptr->common_theme.softkey.button[2].button_type);
			if (need_update_left)
			{
                GUIWIN_SeSoftkeytButtonTextId(base_ctrl_ptr->win_handle,left_softkey_id,0,!need_update_mid && !need_update_right);
			} 
			else
			{
                GUIWIN_SeSoftkeytButtonIconId(base_ctrl_ptr->win_handle,
                baseedit_ctrl_ptr->common_theme.softkey.button[0].image_id,0,TRUE);
			}
			if (need_update_mid)
			{
				GUIWIN_SeSoftkeytButtonTextId(base_ctrl_ptr->win_handle,mid_softkey_id,1,!need_update_right);
			} 
			else
			{
                GUIWIN_SeSoftkeytButtonIconId(base_ctrl_ptr->win_handle,
                baseedit_ctrl_ptr->common_theme.softkey.button[1].image_id,1,TRUE);
			}
			if (need_update_right)
			{
				GUIWIN_SeSoftkeytButtonTextId(base_ctrl_ptr->win_handle,right_softkey_id,2,TRUE);
			} 
			else
			{
                GUIWIN_SeSoftkeytButtonIconId(base_ctrl_ptr->win_handle,
                baseedit_ctrl_ptr->common_theme.softkey.button[2].image_id,2,TRUE);
			}
#else		
			if (EDIT_BUTTON_TEXT_ID == baseedit_ctrl_ptr->common_theme.softkey.button[0].button_type)
			{
                GUIWIN_SeSoftkeytButtonTextId(base_ctrl_ptr->win_handle,left_softkey_id,0,TRUE);
			} 
			else
			{
                GUIWIN_SeSoftkeytButtonIconId(
                    base_ctrl_ptr->win_handle,
                    baseedit_ctrl_ptr->common_theme.softkey.button[0].image_id,0,TRUE);
			}
			if (EDIT_BUTTON_TEXT_ID == baseedit_ctrl_ptr->common_theme.softkey.button[1].button_type)
			{
				GUIWIN_SeSoftkeytButtonTextId(base_ctrl_ptr->win_handle,mid_softkey_id,1,TRUE);
			} 
			else
			{
                GUIWIN_SeSoftkeytButtonIconId(
                    base_ctrl_ptr->win_handle,
                    baseedit_ctrl_ptr->common_theme.softkey.button[1].image_id,1,TRUE);
			}
			if (EDIT_BUTTON_TEXT_ID == baseedit_ctrl_ptr->common_theme.softkey.button[2].button_type)
			{
				GUIWIN_SeSoftkeytButtonTextId(base_ctrl_ptr->win_handle,right_softkey_id,2,TRUE);
			} 
			else
			{
                GUIWIN_SeSoftkeytButtonIconId(
                    base_ctrl_ptr->win_handle,
                    baseedit_ctrl_ptr->common_theme.softkey.button[2].image_id,2,TRUE);
			}
#endif /* #if defined(SCRIPTEASY_SUPPORT) */
            is_softkey_set = TRUE;
        }
#else

        if (!is_softkey_set)
        {
            if (0 != left_softkey_id)
            {
                GUIWIN_SetSoftkeyBtnGray (
                    base_ctrl_ptr->win_handle, LEFT_BUTTON,
                    MMIRES_IsTextNull (left_softkey_id));
            }

            if (0 != mid_softkey_id)
            {
                GUIWIN_SetSoftkeyBtnGray (
                    base_ctrl_ptr->win_handle, MIDDLE_BUTTON,
                    MMIRES_IsTextNull (mid_softkey_id));
            }

            if (0 != right_softkey_id)
            {
                GUIWIN_SetSoftkeyBtnGray (
                    base_ctrl_ptr->win_handle, RIGHT_BUTTON,
                    MMIRES_IsTextNull (right_softkey_id));
            }
        }

#endif
    }
}

/*****************************************************************************/
//  Description : reset edit softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_ResetSoftkey (
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr
)
{
    MMI_TEXT_ID_T   left_softkey_id = 0;
    MMI_TEXT_ID_T   mid_softkey_id = 0;
    MMI_TEXT_ID_T   right_softkey_id = 0;
#ifndef GUIF_SOFTKEY_PDASTYLE
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;
#endif

    //get default softkey
    GetDefaultSoftkey (baseedit_ctrl_ptr);

    //set softkey
    left_softkey_id  = baseedit_ctrl_ptr->common_theme.softkey.button[0].text_1;
    mid_softkey_id   = baseedit_ctrl_ptr->common_theme.softkey.button[1].text_1;
    right_softkey_id = baseedit_ctrl_ptr->common_theme.softkey.button[2].text_1;

#ifndef GUIF_SOFTKEY_PDASTYLE
    //display softkey
	if (EDIT_BUTTON_TEXT_ID == baseedit_ctrl_ptr->common_theme.softkey.button[0].button_type)
	{
		GUIWIN_SeSoftkeytButtonTextId(base_ctrl_ptr->win_handle,left_softkey_id,0,TRUE);
	} 
	else
	{
        GUIWIN_SeSoftkeytButtonIconId(
            base_ctrl_ptr->win_handle,
            baseedit_ctrl_ptr->common_theme.softkey.button[0].image_id,0,TRUE);
	}
	if (EDIT_BUTTON_TEXT_ID == baseedit_ctrl_ptr->common_theme.softkey.button[1].button_type)
	{
		GUIWIN_SeSoftkeytButtonTextId(base_ctrl_ptr->win_handle,mid_softkey_id,1,TRUE);
	} 
	else
	{
        GUIWIN_SeSoftkeytButtonIconId(
            base_ctrl_ptr->win_handle,
            baseedit_ctrl_ptr->common_theme.softkey.button[1].image_id,1,TRUE);
	}
	if (EDIT_BUTTON_TEXT_ID == baseedit_ctrl_ptr->common_theme.softkey.button[2].button_type)
	{
		GUIWIN_SeSoftkeytButtonTextId(base_ctrl_ptr->win_handle,right_softkey_id,2,TRUE);
	} 
	else
	{
        GUIWIN_SeSoftkeytButtonIconId(
            base_ctrl_ptr->win_handle,
            baseedit_ctrl_ptr->common_theme.softkey.button[2].image_id,2,TRUE);
	}
#endif
}

/*****************************************************************************/
//  Description : softkey is valid
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsValidSoftkey (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr,
    uint16                 softkey_index
)
{
    BOOLEAN         result = TRUE;
    MMI_TEXT_ID_T   softkey_id[GUISOFTKEY_BUTTON_NUM] = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(GUISOFTKEY_BUTTON_NUM > softkey_index); /*assert verified*/
    if (GUISOFTKEY_BUTTON_NUM <= softkey_index)
    {
        return FALSE;
    }

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT

    // 由于PUBWIN是PDA风格的话，softkey是由PUBWIN创建的BUTTON，所以设置
    // softkey就应该调用pubwin的接口
    if (MMK_GetWinDisplayStyleState (base_ctrl_ptr->win_handle, WS_HAS_PROMPT_WIN))
    {
        BOOLEAN is_softkey_gray[GUISOFTKEY_BUTTON_NUM] = {0};

        MMIPUB_GetWinSoftkeyGray (
            base_ctrl_ptr->win_handle,
            &is_softkey_gray[0],
            &is_softkey_gray[1],
            &is_softkey_gray[2]);

        result = is_softkey_gray[softkey_index] ? FALSE : TRUE;
    }
    else
#endif

        //get current softkey
        if (GUIWIN_GetSoftkeyTextId (base_ctrl_ptr->win_handle,
                                     &softkey_id[0],
                                     &softkey_id[1],
                                     &softkey_id[2]))
        {
            if ((0 == softkey_id[softkey_index]) ||
                (MMITHEME_GetInvalidTextID() == softkey_id[softkey_index]))
            {
                result = FALSE;
            }
        }

    return (result);
}

/*****************************************************************************/
//  Description : get default softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetDefaultSoftkey (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
)
{
    MMI_TEXT_ID_T   left_softkey_id = 0;
    MMI_TEXT_ID_T   mid_softkey_id = 0;
    MMI_TEXT_ID_T   right_softkey_id = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

// 非PDA风格下的PUBWIN，由于softkey的button不走焦点，需要由edit来控制
#if defined(MMIPUBWIN_PDASTYLE_SUPPORT)

    // 由于PUBWIN是PDA风格的话，softkey是由PUBWIN创建的BUTTON，所以设置
    // softkey就应该调用pubwin的接口
    if (MMK_GetWinDisplayStyleState (base_ctrl_ptr->win_handle, WS_HAS_PROMPT_WIN))
    {
        extern void MMIPUB_GetWinSoftkey (
            MMI_WIN_ID_T          win_id,
            MMI_TEXT_ID_T         *left_sk_id_ptr,
            MMI_TEXT_ID_T         *middle_sk_id_ptr,
            MMI_TEXT_ID_T         *right_sk_id_ptr
        );/*lint -esym(526,MMIPUB_GetWinSoftkey) */

        MMIPUB_GetWinSoftkey (
                base_ctrl_ptr->win_handle,
                &left_softkey_id,
                &mid_softkey_id,
                &right_softkey_id);
    }
    else
#endif
    {
        //get current softkey
        GUIWIN_GetSoftkeyTextId (
                base_ctrl_ptr->win_handle,
                &left_softkey_id,
                &mid_softkey_id,
                &right_softkey_id);
    }

    if (!baseedit_ctrl_ptr->common_theme.softkey.button[0].is_modify)
    {
        //set softkey
        baseedit_ctrl_ptr->common_theme.softkey.button[0].text_1 = left_softkey_id;

        //next not need get softkey
        baseedit_ctrl_ptr->common_theme.softkey.button[0].is_modify = TRUE;
    }

    if (!baseedit_ctrl_ptr->common_theme.softkey.button[1].is_modify)
    {
        //set softkey
        baseedit_ctrl_ptr->common_theme.softkey.button[1].text_1 = mid_softkey_id;

        //next not need get softkey
        baseedit_ctrl_ptr->common_theme.softkey.button[1].is_modify = TRUE;
    }

    if (!baseedit_ctrl_ptr->common_theme.softkey.button[2].is_modify)
    {
        //set softkey
        baseedit_ctrl_ptr->common_theme.softkey.button[2].text_1 = right_softkey_id;

        //next not need get softkey
        baseedit_ctrl_ptr->common_theme.softkey.button[2].is_modify = TRUE;
    }
}

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
/*****************************************************************************/
//  Description : 判断button是否灰化
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSoftkeyButtonGray (
    MMI_TEXT_ID_T     text_id
)
{
    return (MMITHEME_GetInvalidTextID() == text_id || 0 == text_id) ? TRUE : FALSE;
}

//#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 获取softkey的内容
//  Global resource dependence :
//  Author: Jassmine
//  Note:
// 为保证一定能显示内容，这里判断下两个text id，获取有效的值
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetSoftkeyButtonText (
    MMI_TEXT_ID_T  cur_text_id,
    MMI_TEXT_ID_T  text_id_1,
    MMI_TEXT_ID_T  text_id_2
)
{
    if (MMITHEME_GetInvalidTextID() == cur_text_id || 0 == cur_text_id)
    {
        if (MMITHEME_GetInvalidTextID() == text_id_1 &&
            MMITHEME_GetInvalidTextID() != text_id_2)
        {
            cur_text_id = text_id_2;
        }
        else if (MMITHEME_GetInvalidTextID() != text_id_1 &&
                 MMITHEME_GetInvalidTextID() == text_id_2)
        {
            cur_text_id = text_id_1;
        }
    }

    return cur_text_id;
}
//#endif  // MMI_PDA_SUPPORT
#endif  // MMIPUBWIN_PDASTYLE_SUPPORT

