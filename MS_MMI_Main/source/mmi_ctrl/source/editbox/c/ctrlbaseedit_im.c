/*****************************************************************************
** File Name:      ctrlbaseedit_im.c                                         *
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
#include "ctrlim.h"
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

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get im content data
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetImContentData (
    MMI_CTRL_ID_T   ctrl_id,
    GUIIM_CONTEXT_T *context_ptr
);

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set edit im
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:允许所有的输入法,初始输入法是智能输入
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetIm (
    MMI_CTRL_ID_T     ctrl_id,    //in
    GUIIM_TYPE_T      allow_im,   //in:允许的输入集合
    GUIIM_TYPE_T      init_im     //in:初始输入法
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);

    if (PNULL != baseedit_ctrl_ptr)
    {
        VTLBASEEDIT_SetImInfo (baseedit_ctrl_ptr, PNULL, &init_im, &allow_im, PNULL, PNULL, PNULL, PNULL);

        if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
        {
            //set init im
            GUIIM_SetType (baseedit_ctrl_ptr->im_ctrl_handle, init_im);

            //set allow im
            GUIIM_SetTypeSet (baseedit_ctrl_ptr->im_ctrl_handle, allow_im);

            //resize edit rect
            BASEEDIT_ResizeRect (baseedit_ctrl_ptr);
        }
    }
}

///*****************************************************************************/
////  Description : set edit im capital mode
////  Global resource dependence :
////  Author: Jassmine
////  Note:
///*****************************************************************************/
//PUBLIC void CTRLBASEEDIT_SetImCapital (
//    MMI_CTRL_ID_T			ctrl_id,	//in
//    GUIIM_CAPITAL_MODE_T	capital_mode//in:大小写模式
//)
//{
//    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

//    //get edit pointer by control id
//    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);

//    if (PNULL != baseedit_ctrl_ptr)
//    {
//        VTLBASEEDIT_SetImInfo (baseedit_ctrl_ptr, PNULL, PNULL, PNULL, PNULL, PNULL, &capital_mode, PNULL);

//        if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
//        {
//            //set im capital
//            GUIIM_SetCapital (baseedit_ctrl_ptr->im_ctrl_handle, capital_mode);
//        }
//    }
//}

///*****************************************************************************/
////  Description : set edit im input type
////  Global resource dependence :
////  Author: Jassmine
////  Note:
///*****************************************************************************/
//PUBLIC void CTRLBASEEDIT_SetImInput (
//    MMI_CTRL_ID_T		ctrl_id,		//in
//    GUIIM_INPUT_TYPE_T	init_input_type,//in:初始输入法类型
//    GUIIM_INPUT_TYPE_T	allow_input_type//in:允许的输入类型集合
//)
//{
//    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

//    //get edit pointer by control id
//    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);

//    if (PNULL != baseedit_ctrl_ptr)
//    {
//        VTLBASEEDIT_SetImInfo (baseedit_ctrl_ptr, PNULL, PNULL, PNULL, &init_input_type, &allow_input_type, PNULL, PNULL);

//        if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
//        {
//            //set init im input type
//            GUIIM_SetInput (baseedit_ctrl_ptr->im_ctrl_handle, init_input_type);

//            //set allow im input type
//            GUIIM_SetInputSet (baseedit_ctrl_ptr->im_ctrl_handle, allow_input_type);

//            //resize edit rect
//            BASEEDIT_ResizeRect (baseedit_ctrl_ptr);
//        }
//    }
//}

/*****************************************************************************/
//  Description : set edit im is enable tp
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:是否允许TP IM
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImTp (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_enable
)
{
    GUIIM_INPUT_TYPE_T  allow_input_type = GUIIM_INPUT_TYPE_ALL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);

    if ((PNULL != baseedit_ctrl_ptr) && (!is_enable))
    {
        allow_input_type = GUIIM_INPUT_TYPE_KEYBOARD | GUIIM_INPUT_TYPE_QWERT; /*lint !e655*/

        VTLBASEEDIT_SetImInfo (baseedit_ctrl_ptr, PNULL, PNULL, PNULL, PNULL, &allow_input_type, PNULL, PNULL);

        if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
        {
            GUIIM_SetInputSet (baseedit_ctrl_ptr->im_ctrl_handle, allow_input_type);

            //resize edit rect
            BASEEDIT_ResizeRect (baseedit_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : set edit im is enable hand writing
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:是否允许IM hand writing
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImHw (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_enable
)
{
    BOOLEAN                 is_forbid_symbol = FALSE;
    GUIIM_TYPE_T            init_im = GUIIM_TYPE_SMART;
    GUIIM_TYPE_T            allow_im = GUIIM_TYPE_SET_ALL;
    GUIIM_INPUT_TYPE_T      init_input = GUIIM_INPUT_TYPE_HANDWRITING;
    GUIIM_INPUT_TYPE_T      allow_input = GUIIM_INPUT_TYPE_HANDWRITING;
    GUIIM_CAPITAL_MODE_T    capital_mode = GUIIM_MODE_LEAD_UPPER;
    GUIIM_TAG_T             tag = GUIIM_TAG_NONE;
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);

    if ((PNULL != baseedit_ctrl_ptr) && (!is_enable))
    {
        //get allow input type
        VTLBASEEDIT_GetImInfo (
            baseedit_ctrl_ptr,
            &is_forbid_symbol,
            &init_im,
            &allow_im,
            &init_input,
            &allow_input,
            &capital_mode,
            &tag);

        //not hand writing
        if (allow_input & GUIIM_INPUT_TYPE_HANDWRITING)
        {
            allow_input = (GUIIM_INPUT_TYPE_T) (allow_input ^ GUIIM_INPUT_TYPE_HANDWRITING);
        }

        VTLBASEEDIT_SetImInfo (baseedit_ctrl_ptr, PNULL, PNULL, PNULL, PNULL, &allow_input, PNULL, PNULL);

        if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
        {
            GUIIM_SetInputSet (baseedit_ctrl_ptr->im_ctrl_handle, allow_input);

            //resize edit rect
            BASEEDIT_ResizeRect (baseedit_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : set edit im is forbid symbo;
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImSymbol (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_forbid
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);

    if (PNULL != baseedit_ctrl_ptr)
    {
        //set im
        VTLBASEEDIT_SetImInfo (baseedit_ctrl_ptr, &is_forbid, PNULL, PNULL, PNULL, PNULL, PNULL, PNULL);

        if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
        {
            GUIIM_SetInputSymbol (baseedit_ctrl_ptr->im_ctrl_handle, is_forbid);
        }
    }
}

/*****************************************************************************/
//  Description : set edit im tag
//  Global resource dependence :
//  Author: Jassmine
//  Note: 比如是email输入的时候，一些输入法会定制一些键盘
//      建议在激活edit控件之前设置，因为在输入法创建之后设置，会重建输入法
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImTag (
    MMI_CTRL_ID_T  ctrl_id,
    GUIIM_TAG_T    tag
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);

    if (PNULL != baseedit_ctrl_ptr)
    {
        //set im
        VTLBASEEDIT_SetImInfo (baseedit_ctrl_ptr, PNULL, PNULL, PNULL, PNULL, PNULL, PNULL, &tag);

        if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
        {
            GUIIM_SetImTag (baseedit_ctrl_ptr->im_ctrl_handle, tag);
        }
    }
}

/*****************************************************************************/
//  Description : config edit im custom key
//  Global resource dependence :
//  Author: Jassmine
//  Note: must < 9 ,only config digital or multi-tap im mode
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_ConfigImCustomKey (
    MMI_CTRL_ID_T         ctrl_id,    //in:
    GUIIM_TYPE_T          im_type,    //in:only digital or multi-tap
    GUIIM_CUSTOM_KEY_T    custom_key, //in:
    wchar                 *value_ptr, //in:
    uint32                value_count //in:
)
{
    GUIEDIT_CONFIG_KEY_T    *new_config_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr = PNULL;

    SCI_ASSERT ( (GUIIM_TYPE_DIGITAL == im_type) || (GUIIM_TYPE_ABC == im_type)); /*assert verified*/

    //get edit pointer by control id
    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);

    if (PNULL != baseedit_ctrl_ptr)
    {
        //alloc config memory
        new_config_ptr = SCI_ALLOC_APP (sizeof (GUIEDIT_CONFIG_KEY_T));
        SCI_MEMSET (new_config_ptr, 0, sizeof (GUIEDIT_CONFIG_KEY_T));

        new_config_ptr->value_ptr = SCI_ALLOC_APP (value_count * sizeof (wchar));
        SCI_MEMSET (new_config_ptr->value_ptr, 0, (value_count*sizeof (wchar)));

        //set param
        MMI_MEMCPY (new_config_ptr->value_ptr, (value_count*sizeof (wchar)),
                    value_ptr, (value_count*sizeof (wchar)),
                    (value_count*sizeof (wchar)));
        new_config_ptr->value_count = value_count;
        new_config_ptr->im_type     = im_type;
        new_config_ptr->custom_key  = custom_key;

        //save config key info
        if (PNULL == baseedit_ctrl_ptr->im_config_key_ptr)
        {
            //set new config previous and next
            new_config_ptr->prev_ptr = new_config_ptr;
            new_config_ptr->next_ptr = PNULL;

            //set first config
            baseedit_ctrl_ptr->im_config_key_ptr = new_config_ptr;
        }
        else
        {
            //set new config previous and next
            new_config_ptr->prev_ptr = baseedit_ctrl_ptr->im_config_key_ptr->prev_ptr;
            new_config_ptr->next_ptr = PNULL;

            //set previous config next
            baseedit_ctrl_ptr->im_config_key_ptr->prev_ptr->next_ptr = new_config_ptr;

            //set first config previous
            baseedit_ctrl_ptr->im_config_key_ptr->prev_ptr = new_config_ptr;
        }

        if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
        {
            GUIIM_SetCustomKey (baseedit_ctrl_ptr->im_ctrl_handle, im_type, custom_key, value_ptr, value_count);
        }
    }
}

/*****************************************************************************/
//  Description: check whether im is open
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_IsImOpen (
    MMI_CTRL_ID_T ctrl_id
)
{
    BOOLEAN result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        result = (0 != baseedit_ctrl_ptr->im_ctrl_handle) ? TRUE : FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description: judge whether application can set softkey or not
//  Author: arvin.zhang
//  Note:app cann't modify softkey if im is working on several state
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_IsImNeedSoftkey (
									  MMI_CTRL_ID_T ctrl_id
									  )
{
	BOOLEAN			   result			  = FALSE;
	CTRLIM_OBJ_T	   *im_ctrl_ptr		  = PNULL;
    CTRLBASEEDIT_OBJ_T *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);

	if (PNULL != baseedit_ctrl_ptr)
	{
		im_ctrl_ptr = (CTRLIM_OBJ_T *) MMK_GetCtrlPtr (baseedit_ctrl_ptr->im_ctrl_handle);
		
		if (PNULL != im_ctrl_ptr)
		{
			result = (im_ctrl_ptr->edit_state_bak & GUIIM_EDIT_SELECTING)
				|| (im_ctrl_ptr->edit_state_bak & GUIIM_EDIT_CANDITATE)
				|| (im_ctrl_ptr->edit_state_bak & GUIIM_INPUT_TYPE_HANDWRITING);
		}
	}
	
    return result;
}

/*****************************************************************************/
//  Description : when edit get active, is create im
//  Global resource dependence :
//  Author: Jassmine
//  Note: default TRUE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_CreateImEditActive (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    BOOLEAN           is_create   //in:
)
{
    BOOLEAN         result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        baseedit_ctrl_ptr->is_create_im_active = is_create;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description: judge whether the TP panel is visible
//  Author: wancan.you
//  Param
//       ctrl_id :control id
//  Return:
//       TRUE:visible;FALSE:invisible
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_IsTPPanelVisible (
    MMI_CTRL_ID_T ctrl_id
)
{
    BOOLEAN result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
#if 0
        GUIIM_INPUT_TYPE_T default_input = GUIIM_INPUT_TYPE_NONE;

        result = !GUIIM_CanSwithToTp (baseedit_ctrl_ptr->im_ctrl_handle, &default_input);
#else
        CTRLIM_OBJ_T * im_ctrl_ptr = (CTRLIM_OBJ_T *) MMK_GetCtrlPtr (baseedit_ctrl_ptr->im_ctrl_handle);

        if (PNULL != im_ctrl_ptr)
        {
            result = (im_ctrl_ptr->method_param.im_set.input & GUIIM_INPUT_TYPE_TOUCHPEN)
                     || (im_ctrl_ptr->method_param.im_set.input & GUIIM_INPUT_TYPE_TP_QWERT)
                     || (im_ctrl_ptr->method_param.im_set.input & GUIIM_INPUT_TYPE_HANDWRITING);
        }

#endif
    }

    return result;
}

/*****************************************************************************/
//  Description : get im handle
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLBASEEDIT_GetIMHandle(
                                        MMI_CTRL_ID_T     ctrl_id    //in:
                                        )
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;
    MMI_HANDLE_T im_handle = 0;

    baseedit_ctrl_ptr = BASEEDIT_GetPtr (ctrl_id);
    if (PNULL != baseedit_ctrl_ptr)
    {
        im_handle = baseedit_ctrl_ptr->im_ctrl_handle;
    }

    return im_handle;
}

/*****************************************************************************/
//  Description : create im control and set it active
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_CreateImCtrl (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    BOOLEAN                 result = FALSE;
    CTRLIM_OBJ_T            *im_ctrl_ptr = PNULL;
    GUIIM_INIT_PARAM_T      init_data = {0};
    MMI_CONTROL_CREATE_T    create_im = {0};
    GUIEDIT_CONFIG_KEY_T    *im_config_ptr = PNULL;
    CTRLBASE_OBJ_T          *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;
    CTRLBASE_OBJ_T          *ctrl_ptr = PNULL;

    // touch风格不需要输入法
    if (0 == baseedit_ctrl_ptr->im_ctrl_handle)
    {
        //get im info
        VTLBASEEDIT_GetImInfo (
            baseedit_ctrl_ptr,
            &init_data.is_ban_symbol,
            &init_data.im_type,
            &init_data.im_type_set,
            &init_data.input_type,
            &init_data.input_type_set,
            &init_data.cap_mode,
            &init_data.tag);

        init_data.f_get_context = GetImContentData;

        //create im
        create_im.ctrl_id            = 0;
        create_im.guid               = SPRD_GUI_IM_ID;
        create_im.init_data_ptr      = &init_data;
        create_im.parent_ctrl_handle = base_ctrl_ptr->handle;
        create_im.parent_win_handle  = base_ctrl_ptr->win_handle;

        im_ctrl_ptr = (CTRLIM_OBJ_T *) MMK_CreateControl (&create_im);
        SCI_ASSERT (PNULL != im_ctrl_ptr); /*assert verified*/

        ctrl_ptr = (CTRLBASE_OBJ_T *)im_ctrl_ptr;
        // 设置im控件layer与editbox layer相同
        IGUICTRL_SetLcdDevInfo ( (IGUICTRL_T*) im_ctrl_ptr, &base_ctrl_ptr->lcd_dev_info);

        baseedit_ctrl_ptr->im_ctrl_handle = ctrl_ptr->handle;

// fix NEWMS00124665: PDA项目的手写不是全屏的，不用TOPMOST
#if defined(MMIIM_HW_FULLSCREEN_SUPPORT)

        //set hand writing im is top most control
        if ((0 != baseedit_ctrl_ptr->im_ctrl_handle) &&
            (GUIIM_INPUT_TYPE_HANDWRITING == GUIIM_GetInput (baseedit_ctrl_ptr->im_ctrl_handle)))
        {
            IGUICTRL_SetState ( (IGUICTRL_T *) im_ctrl_ptr, GUICTRL_STATE_TOPMOST, TRUE);
        }

#endif

        //config im
        VTLBASEEDIT_ConfigImKey (baseedit_ctrl_ptr);

        //config custom key
        im_config_ptr = baseedit_ctrl_ptr->im_config_key_ptr;

        while (PNULL != im_config_ptr)
        {
            GUIIM_SetCustomKey (baseedit_ctrl_ptr->im_ctrl_handle,
                                im_config_ptr->im_type,
                                im_config_ptr->custom_key,
                                im_config_ptr->value_ptr,
                                im_config_ptr->value_count);

            im_config_ptr = im_config_ptr->next_ptr;
        }

        //set im active
        BASEEDIT_SetImActive (baseedit_ctrl_ptr);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : destroy im control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_DestroyImCtrl (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr //in:
)
{
    BOOLEAN         result = FALSE;
    BOOLEAN         is_notify = FALSE;
    GUI_RECT_T      im_rect = {0};
    MMI_HANDLE_T    ctrl_handle = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
    {
        //destroy progress bar control
        MMK_DestroyControl (baseedit_ctrl_ptr->im_ctrl_handle);
        baseedit_ctrl_ptr->im_ctrl_handle = 0;

        if (BASEEDIT_IsParentCtrl(baseedit_ctrl_ptr))
        {
            if (base_ctrl_ptr->is_active)
            {
                is_notify = TRUE;
            }
            else
            {
                //current active is edit,避免notify消息发送两次
                ctrl_handle = MMK_GetActiveCtrl(base_ctrl_ptr->win_handle);
                if (TYPEMNG_IsTypeOf ((OBJECT_TYPE_PTR)MMK_GetCtrlPtr(ctrl_handle),CTRL_BASEEDIT_TYPE))
                {
                    is_notify = FALSE;
                }
                else
                {
                    is_notify = TRUE;
                }
            }

            if (is_notify)
            {
                //im rect is not in edit
                CTRLMSG_SendNotifyEx (base_ctrl_ptr->handle, MSG_NOTIFY_RESIZE, &im_rect);
            }
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : reset im
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_ResetIm (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    BOOLEAN                 is_forbid_symbol = FALSE;
    GUIIM_TYPE_T            init_im = GUIIM_TYPE_SMART;
    GUIIM_TYPE_T            allow_im = GUIIM_TYPE_SET_ALL;
    GUIIM_INPUT_TYPE_T      init_input = GUIIM_INPUT_TYPE_HANDWRITING;
    GUIIM_INPUT_TYPE_T      allow_input = GUIIM_INPUT_TYPE_HANDWRITING;
    GUIIM_CAPITAL_MODE_T    capital_mode = GUIIM_MODE_LEAD_UPPER;
    GUIIM_TAG_T             tag = GUIIM_TAG_NONE;

    if (PNULL != baseedit_ctrl_ptr && 0 != baseedit_ctrl_ptr->im_ctrl_handle)
    {
        //set im by list style
        VTLBASEEDIT_GetImInfo (
            baseedit_ctrl_ptr,
            &is_forbid_symbol,
            &init_im,
            &allow_im,
            &init_input,
            &allow_input,
            &capital_mode,
            &tag);

        //set init im
        GUIIM_SetType (baseedit_ctrl_ptr->im_ctrl_handle, init_im);

        //set allow im
        GUIIM_SetTypeSet (baseedit_ctrl_ptr->im_ctrl_handle, allow_im);

        //config im
        VTLBASEEDIT_ConfigImKey (baseedit_ctrl_ptr);

        //resize edit rect
        BASEEDIT_ResizeRect (baseedit_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : set im active
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_SetImActive (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

    if ((base_ctrl_ptr->is_active) &&
        (0 != baseedit_ctrl_ptr->im_ctrl_handle))
    {
        //im control not active and edit not full
        if (!MMK_IsActiveCtrl (baseedit_ctrl_ptr->im_ctrl_handle))
        {
            //set im active
            MMK_SetAtvCtrl (base_ctrl_ptr->win_handle, baseedit_ctrl_ptr->im_ctrl_handle);
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get edit im is enable tp
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:是否允许TP IM
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_GetImTp (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
)
{
    BOOLEAN                 result = FALSE;
    BOOLEAN                 is_forbid_symbol = FALSE;
    GUIIM_TAG_T             tag = GUIIM_TAG_NONE;
    GUIIM_TYPE_T            init_im = GUIIM_TYPE_SMART;
    GUIIM_TYPE_T            allow_im = GUIIM_TYPE_SET_ALL;
    GUIIM_INPUT_TYPE_T      init_input = GUIIM_INPUT_TYPE_HANDWRITING;
    GUIIM_INPUT_TYPE_T      allow_input = GUIIM_INPUT_TYPE_HANDWRITING;
    GUIIM_CAPITAL_MODE_T    capital_mode = GUIIM_MODE_LEAD_UPPER;

    //get allow input type
    VTLBASEEDIT_GetImInfo (
        baseedit_ctrl_ptr,
        &is_forbid_symbol,
        &init_im,
        &allow_im,
        &init_input,
        &allow_input,
        &capital_mode,
        &tag);

    if ((allow_input & GUIIM_INPUT_TYPE_TOUCHPEN) ||
        (allow_input & GUIIM_INPUT_TYPE_TP_QWERT) ||
        (allow_input & GUIIM_INPUT_TYPE_HANDWRITING))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : edit handle im switch input type, current im
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_HandleImSwitch (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr  //in:
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseedit_ctrl_ptr;

// fix NEWMS00124665: PDA项目的手写不是全屏的，不用TOPMOST
#if defined(MMIIM_HW_FULLSCREEN_SUPPORT)
    //set hand writing im is top most control
    if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
    {
        BOOLEAN     is_top_most = FALSE;

        if (GUIIM_INPUT_TYPE_HANDWRITING == GUIIM_GetInput (baseedit_ctrl_ptr->im_ctrl_handle))
        {
            is_top_most = TRUE;
        }

        IGUICTRL_SetState (MMK_GetCtrlPtr (baseedit_ctrl_ptr->im_ctrl_handle), GUICTRL_STATE_TOPMOST, is_top_most);
    }
#endif

    //resize edit rect
    BASEEDIT_ResizeRect (baseedit_ctrl_ptr);

    //update window
    if (MMK_IsFocusWin (base_ctrl_ptr->win_handle))
    {
        MMK_SendMsg (base_ctrl_ptr->win_handle, MSG_FULL_PAINT, PNULL);
    }
}

/*****************************************************************************/
//  Description : convert commit msg to GUIEDIT_ADD_STR_T
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_ConvertImCommitMsg(
    DPARAM              param,              // in
    GUIEDIT_ADD_STR_T   *commit_str_ptr     // out
)
{
    if (PNULL != commit_str_ptr)
    {
        //set add string
        commit_str_ptr->is_im_commit  = TRUE;
        commit_str_ptr->commit_ptr    = ( (GUIIM_NOTIFY_PARAM_T *) param)->commit;
        commit_str_ptr->commit_len    = ( (GUIIM_NOTIFY_PARAM_T *) param)->commit_len;
        commit_str_ptr->underline_ptr = ( (GUIIM_NOTIFY_PARAM_T *) param)->underline;
        commit_str_ptr->underline_len = ( (GUIIM_NOTIFY_PARAM_T *) param)->underline_len;
    }
}

/*****************************************************************************/
//  Description : get im content data
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetImContentData (
    MMI_CTRL_ID_T   edit_ctrl_id,
    GUIIM_CONTEXT_T *context_ptr
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;
    
    //get edit pointer by control id
    baseedit_ctrl_ptr = BASEEDIT_GetPtr (edit_ctrl_id);

    if (PNULL != baseedit_ctrl_ptr)
    {
        context_ptr->str_ptr = baseedit_ctrl_ptr->str_ptr;
        context_ptr->str_len = baseedit_ctrl_ptr->str_len;
        context_ptr->pos = baseedit_ctrl_ptr->cursor.cur_pos; 

        //highlight info
        context_ptr->is_underline = baseedit_ctrl_ptr->highlight.is_underline;
        context_ptr->start_pos = baseedit_ctrl_ptr->highlight.start_pos;
        context_ptr->end_pos = baseedit_ctrl_ptr->highlight.end_pos;
        
        result = TRUE;
    }

    return (result);
}

