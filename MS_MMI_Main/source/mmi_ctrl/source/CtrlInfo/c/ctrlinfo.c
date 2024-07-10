/*****************************************************************************
** File Name:      ctrlinfo.c                                                *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2009      Jassmine              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "caf.h"
#include "cafcontrol.h"
#include "mmk_handle_internal.h"
#include "type_manager.h"

#include "ctrlbase.h"
#include "ctrlbutton.h"
#include "ctrlcheckbox.h"
#include "ctrldropdownlist.h"
#include "ctrlform.h"
#include "ctrllabel.h"
#include "ctrllistbox.h"
#include "ctrlmsgbox.h"
#include "ctrlownerdraw.h"
#include "ctrlprogressbar.h"
#include "ctrltext.h"
#include "ctrltips.h"
#ifdef GUIF_TOOLBAR     
#include "ctrltoolbar.h"
#endif
#include "ctrlmenu.h"
#include "ctrlslider.h"
#include "ctrlprogress.h"
#include "ctrllettersbar.h"
#include "ctrlscroll.h"
#include "ctrlbasepicker.h"
#include "ctrltimepicker.h"
#include "ctrldatepicker.h"
#include "ctrlpicker.h"
#include "ctrlcombox.h"
#include "ctrltextedit.h"
#include "ctrldigitaledit.h"
#include "ctrlphonenumedit.h"
#include "ctrlpswedit.h"
#include "ctrllistedit.h"
#include "ctrldateedit.h"
#include "ctrltimeedit.h"
#include "ctrlipedit.h"
#include "ctrlrichtext.h"
#include "ctrltitle.h"
#include "ctrlsoftkey.h"
#include "ctrlsetlist.h"
#include "ctrlim.h"
#include "ctrlstatusbar.h"
#include "ctrltab.h"
#include "ctrlmainmenu.h"

#include "guictrl.h"
#include "cafctrledit.h"
#include "cafctrllist.h"
#include "cafctrlanim.h"
#include "cafctrllabel.h"
#include "cafctrlmenu.h"
#include "cafctrliconlist.h"
#include "cafctrldropdownlist.h"
#include "cafctrlbutton.h"
#include "cafctrltextbox.h"
#include "mmk_handle_internal.h"

#include "ctrlanim.h"
#include "ctrliconlist.h"
#ifdef GUIF_ICONFOLDER
#include "ctrliconfolder.h"
#endif
#ifdef GUIF_SCROLLKEY
#include "ctrlscrollkey.h"
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef TYPE_TAG_T (*GET_CTRL_TYPE)(void);

// 控件的信息
typedef struct
{
    CAF_GUID_T      guid;       /*!< 控件id*/ 
    const char*     name;
    GET_CTRL_TYPE   ctrl_type;
} GUICTRL_CLASS_INFO_T;

/*! @struct GUICTRL_CLASS_INFO_T
@brief 控件的信息
*/
typedef struct
{
    CAF_GUID_T  guid;       /*!< 控件id*/ 
    const char* name;
    uint32      size;       /*!< 控件尺寸*/ 
    void        *vtbl_ptr;/*!< 虚表指针*/
} GUICTRL_CLASS_INFO_EX_T;

typedef union
{
    GUITIMEPICKER_INIT_DATA_T   timepicker_init_data;
    GUIDATEPICKER_INIT_DATA_T   datepicker_init_data;
    GUIPICKER_INIT_DATA_T       picker_init_data;
} CTRLINFO_PARAM_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : new gui control
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CtrlClassNew(
                                CAF_GUID_T  guid,
                                void        **object_pptr 
                                );

/*****************************************************************************/
// 	Description : notify parent ctrl
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetParentHandle(
                                   MMI_HANDLE_T ctrl_handle
                                   );

/*****************************************************************************/
//  Description : construct 
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN VTLCTRL_Construct(
                              IGUICTRL_T    *iguictrl_ptr,
                              void          *init_data_ptr
                              );

/*****************************************************************************/
//  Description : destruct 
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN VTLCTRL_Destruct(
                              IGUICTRL_T    *iguictrl_ptr
                              );

LOCAL MMI_RESULT_E VTLCTRL_HandleMsg(
                              IGUICTRL_T            *iguictrl_ptr,
                              CAF_MESSAGE_ID_E      msg_id,
                              void*                 param_ptr
                              );

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL const CAF_VTBL_T(IGUICTRL_T) s_ctrl_vtbl =
{
    VTLCTRL_QueryInterface,
    VTLCTRL_AddRef,
    VTLCTRL_Release,
    VTLCTRL_Construct,
    VTLCTRL_Destruct,
    VTLCTRL_HandleMsg,
    VTLCTRL_GetCtrlGuid,
    VTLCTRL_SetCtrlHandle,
    VTLCTRL_GetCtrlHandle,
    VTLCTRL_SetState,
    VTLCTRL_GetState,
    VTLCTRL_SetRect,
    VTLCTRL_GetRect,
    VTLCTRL_SetLcdDevInfo,
    VTLCTRL_GetLcdDevInfo,

    //新增基类接口
    VTLCTRL_SetProperties,
    VTLCTRL_GetProperties,
    VTLCTRL_TestProperties,
    VTLCTRL_Redraw,
    VTLCTRL_SetActive,
    VTLCTRL_IsActive,
    VTLCTRL_SetBorder,
    VTLCTRL_SetBg,
    VTLCTRL_SetFont,
    VTLCTRL_GetFont,
    VTLCTRL_SetProgress,
    VTLCTRL_SetCircularHandleUpDown,
    VTLCTRL_SetCircularHandleLeftRight,
    VTLCTRL_GetHeightByWidth,
    VTLCTRL_GetDisplayRect,
    VTLCTRL_SetDisplayRect,
    VTLCTRL_GetSelectRect,
    VTLCTRL_GetSelectIndex,
    VTLCTRL_SetSelectIndex,
    VTLCTRL_GetBothRect,
    VTLCTRL_SetBothRect,
};

// 定义控件中类的信息表
LOCAL GUICTRL_CLASS_INFO_T s_ctrl_class_info[] =
{
    {GUID_NAME_DEF(SPRD_GUI_TEXTEDIT_ID),      TEXTEDIT_GetType},
    {GUID_NAME_DEF(SPRD_GUI_PHONENUMEDIT_ID),  PHONENUMEDIT_GetType},
    {GUID_NAME_DEF(SPRD_GUI_DIGITALEDIT_ID),   DIGITALEDIT_GetType},
    {GUID_NAME_DEF(SPRD_GUI_PSWEDIT_ID),       PSWEDIT_GetType},
    {GUID_NAME_DEF(SPRD_GUI_LISTEDIT_ID),      LISTEDIT_GetType},
    {GUID_NAME_DEF(SPRD_GUI_DATEEDIT_ID),      DATEEDIT_GetType},
    {GUID_NAME_DEF(SPRD_GUI_TIMEEDIT_ID),      TIMEEDIT_GetType},
    {GUID_NAME_DEF(SPRD_GUI_IPEDIT_ID),        IPEDIT_GetType},
    {GUID_NAME_DEF(SPRD_GUI_LIST_ID),          LIST_GetType},
    {GUID_NAME_DEF(SPRD_GUI_MENU_ID),          MENU_GetType},
    {GUID_NAME_DEF(SPRD_GUI_MSGBOX_ID),        MSGBOX_GetType},
    {GUID_NAME_DEF(SPRD_GUI_PRGBOX_ID),        CTRLPROGRESSBAR_GetType},
    {GUID_NAME_DEF(SPRD_GUI_TEXTBOX_ID),       TEXT_GetType},
    {GUID_NAME_DEF(SPRD_GUI_LABEL_ID),         LABEL_GetType},
    {GUID_NAME_DEF(SPRD_GUI_BUTTON_ID),        BUTTON_GetType},
    {GUID_NAME_DEF(SPRD_GUI_CHECKBOX_ID),      CHECKBOX_GetType},
    {GUID_NAME_DEF(SPRD_GUI_DROPDOWNLIST_ID),  DROPDOWNLIST_GetType},
    {GUID_NAME_DEF(SPRD_GUI_TIPS_ID),          CTRLTIPS_GetType},
    {GUID_NAME_DEF(SPRD_GUI_FORM_ID),          FORM_GetType},
    {GUID_NAME_DEF(SPRD_GUI_OWNDRAW_ID),       CTRLOWNERDRAW_GetType},
#ifdef GUIF_TOOLBAR     
    {GUID_NAME_DEF(SPRD_GUI_TOOLBAR_ID),       TOOLBAR_GetType},
#endif
    {GUID_NAME_DEF(SPRD_GUI_SLIDER_ID),        SLIDER_GetType},
    {GUID_NAME_DEF(SPRD_GUI_PROGRESS_ID),      PROGRESS_GetType},
    {GUID_NAME_DEF(SPRD_GUI_LETTERS_ID),       LETTERSBAR_GetType},
    {GUID_NAME_DEF(SPRD_GUI_SCROLL_ID),        SCROLL_GetType},
    {GUID_NAME_DEF(SPRD_GUI_PICKER_ID),        PICKER_GetType},
    {GUID_NAME_DEF(SPRD_GUI_TIMEPICKER_ID),    TIMEPICKER_GetType},
    {GUID_NAME_DEF(SPRD_GUI_DATEPICKER_ID),    DATEPICKER_GetType},
    {GUID_NAME_DEF(SPRD_GUI_COMBOX_ID),        COMBOX_GetType},
#ifdef GUIF_ICONFOLDER	
    {GUID_NAME_DEF(SPRD_GUI_ICONFOLDER_ID),    ICONFOLDER_GetType},
#endif
	{GUID_NAME_DEF(SPRD_GUI_RICHTEXT_ID),      RICHTEXT_GetType},
#ifdef GUIF_ICONLIST
    {GUID_NAME_DEF(SPRD_GUI_ICONLIST_ID),      ICONLIST_GetType},
#endif	
    {GUID_NAME_DEF(SPRD_GUI_TITLE_ID),         TITLE_GetType},
	{GUID_NAME_DEF(SPRD_GUI_SOFTKEY_ID),       SOFTKEY_GetType},
	{GUID_NAME_DEF(SPRD_GUI_IM_ID),            IM_GetType},
	{GUID_NAME_DEF(SPRD_GUI_SETLIST_ID),       SETLIST_GetType},	
    {GUID_NAME_DEF(SPRD_GUI_STATUSBAR_ID),     STATUSBAR_GetType},
    {GUID_NAME_DEF(SPRD_GUI_TAB_ID),           TAB_GetType},
    {GUID_NAME_DEF(SPRD_GUI_ANIM_ID),          ANIM_GetType},
#ifdef GUIF_SCROLLKEY
    {GUID_NAME_DEF(SPRD_GUI_SCROLLKEY_ID),     SCROLLKEY_GetType},
#endif
    {GUID_NAME_DEF(SPRD_GUI_MAINMENU_ID),      MAINMENU_GetType},
};

LOCAL const CAF_STATIC_CLASS_T s_ctrl_class_list[] = 
{
    {SPRD_GUI_EDITBOX_ID,       CtrlClassNew, 0 },
    {SPRD_GUI_LIST_ID,          CtrlClassNew, 0 },
    {SPRD_GUI_ANIM_ID,          CtrlClassNew, 0 },
    {SPRD_GUI_MAINMENU_ID,      CtrlClassNew, 0 },
    {SPRD_GUI_MENU_ID,          CtrlClassNew, 0 },
    {SPRD_GUI_MSGBOX_ID,        CtrlClassNew, 0 },
    {SPRD_GUI_PRGBOX_ID,        CtrlClassNew, 0 },
    {SPRD_GUI_TEXTBOX_ID,       CtrlClassNew, 0 },
    {SPRD_GUI_LABEL_ID,         CtrlClassNew, 0 },
    {SPRD_GUI_BUTTON_ID,        CtrlClassNew, 0 },
    {SPRD_GUI_CHECKBOX_ID,      CtrlClassNew, 0 },
    {SPRD_GUI_DROPDOWNLIST_ID,  CtrlClassNew, 0 },
#ifdef GUIF_ICONLIST
    {SPRD_GUI_ICONLIST_ID,      CtrlClassNew, 0 },
#endif
    {SPRD_GUI_RICHTEXT_ID,      CtrlClassNew, 0 },
    {SPRD_GUI_TAB_ID,           CtrlClassNew, 0 },
    {SPRD_GUI_STATUSBAR_ID,     CtrlClassNew, 0 },
    {SPRD_GUI_SOFTKEY_ID,       CtrlClassNew, 0 },
    {SPRD_GUI_TIPS_ID,          CtrlClassNew, 0 },
    {SPRD_GUI_TITLE_ID,         CtrlClassNew, 0 },
    {SPRD_GUI_IM_ID,            CtrlClassNew, 0 },
    {SPRD_GUI_SETLIST_ID,       CtrlClassNew, 0 },
    {SPRD_GUI_FORM_ID,          CtrlClassNew, 0 },
    {SPRD_GUI_OWNDRAW_ID,       CtrlClassNew, 0 },
#ifdef GUIF_TOOLBAR
    {SPRD_GUI_TOOLBAR_ID,       CtrlClassNew, 0 },
#endif
#ifdef GUIF_ICONFOLDER	
    {SPRD_GUI_ICONFOLDER_ID,    CtrlClassNew, 0 },
#endif
#ifdef GUIF_SCROLLKEY
    {SPRD_GUI_SCROLLKEY_ID,     CtrlClassNew, 0 },
#endif
	{SPRD_GUI_SLIDER_ID,        CtrlClassNew, 0	},
	{SPRD_GUI_PROGRESS_ID,      CtrlClassNew, 0	},
	{SPRD_GUI_LETTERS_ID,       CtrlClassNew, 0	},
    {SPRD_GUI_SCROLL_ID,        CtrlClassNew, 0 },
	{SPRD_GUI_PICKER_ID,        CtrlClassNew, 0	},
	{SPRD_GUI_TIMEPICKER_ID,    CtrlClassNew, 0	},
	{SPRD_GUI_DATEPICKER_ID,    CtrlClassNew, 0	},
	{SPRD_GUI_COMBOX_ID,        CtrlClassNew, 0	},

	{SPRD_GUI_TEXTEDIT_ID,      CtrlClassNew, 0	},
	{SPRD_GUI_PHONENUMEDIT_ID,  CtrlClassNew, 0	},
	{SPRD_GUI_DIGITALEDIT_ID,   CtrlClassNew, 0	},
    {SPRD_GUI_PSWEDIT_ID,       CtrlClassNew, 0 },
	{SPRD_GUI_LISTEDIT_ID,      CtrlClassNew, 0	},
	{SPRD_GUI_DATEEDIT_ID,      CtrlClassNew, 0	},
	{SPRD_GUI_TIMEEDIT_ID,      CtrlClassNew, 0	},
	{SPRD_GUI_IPEDIT_ID,        CtrlClassNew, 0	},
};

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
const CAF_STATIC_CLASS_INFO_T g_ctrl_static_class_info =
{ 
    (CAF_STATIC_CLASS_T*)s_ctrl_class_list,
    sizeof(s_ctrl_class_list)/sizeof(s_ctrl_class_list[0])
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : new gui control
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CtrlClassNew(
                                CAF_GUID_T  guid,
                                void        **object_pptr 
                                )
{
    uint32              i = 0;
    uint32              class_num = ARR_SIZE(s_ctrl_class_info);
    CAF_RESULT_E        result = CAF_RESULT_NOT_SUPPORT;
    OBJECT_TYPE_T       *guictrl_ptr = PNULL;

    if ((SPRD_GUI_CTRL_GUID_STATR <= guid) &&
        (SPRD_GUI_CTRL_GUID_END >= guid))
    {
        // for new ctrl
        for (i=0; i< class_num; i++)
        {
            if (guid == s_ctrl_class_info[i].guid)
            {
                guictrl_ptr = TYPEMNG_Create(s_ctrl_class_info[i].ctrl_type());

                if (PNULL != guictrl_ptr)
                {
    				guictrl_ptr->vtbl_ptr   = (CAF_VTBL_T(IGUICTRL_T)*)&s_ctrl_vtbl;
    				guictrl_ptr->guid       = guid;
    				guictrl_ptr->ref_num    = 1;

    				result = CAF_RESULT_SUCCEEDED;
                }

                break;
            }
        }
    }

    *object_pptr = guictrl_ptr;

    return (result);
}

/*****************************************************************************/
// 	Description : check ctrl creating param
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_CheckCtrlParam(
    MMI_CONTROL_CREATE_T    *create_ptr
    )
{
    static CTRLINFO_PARAM_T ctrl_param = {0};

    if (PNULL != create_ptr)
    {
        SCI_MEMSET(&ctrl_param, 0, sizeof(ctrl_param));

        if (SPRD_GUI_EDITBOX_ID == create_ptr->guid)
        {
            GUIEDIT_INIT_DATA_T *edit_init_ptr = (GUIEDIT_INIT_DATA_T*)create_ptr->init_data_ptr;

            switch (edit_init_ptr->type)
            {
            case GUIEDIT_TYPE_TOUCH_DATE:
                create_ptr->guid = SPRD_GUI_DATEPICKER_ID;
                ctrl_param.datepicker_init_data.both_rect = edit_init_ptr->both_rect;
                create_ptr->init_data_ptr = (void*)&ctrl_param;
                break;

            case GUIEDIT_TYPE_TOUCH_TIME:
                create_ptr->guid = SPRD_GUI_TIMEPICKER_ID;
                ctrl_param.timepicker_init_data.both_rect = edit_init_ptr->both_rect;
                create_ptr->init_data_ptr = (void*)&ctrl_param;
                break;

            case GUIEDIT_TYPE_PICKER:
                create_ptr->guid = SPRD_GUI_PICKER_ID;
                ctrl_param.picker_init_data.both_rect = edit_init_ptr->both_rect;
                create_ptr->init_data_ptr = (void*)&ctrl_param;
                break;

            case GUIEDIT_TYPE_TEXT:
                create_ptr->guid = SPRD_GUI_TEXTEDIT_ID;
                break;

            case GUIEDIT_TYPE_PHONENUM:
                create_ptr->guid = SPRD_GUI_PHONENUMEDIT_ID;
                break;

            case GUIEDIT_TYPE_DIGITAL:
                create_ptr->guid = SPRD_GUI_DIGITALEDIT_ID;
                break;

            case GUIEDIT_TYPE_PASSWORD:
                create_ptr->guid = SPRD_GUI_PSWEDIT_ID;
                break;

            case GUIEDIT_TYPE_LIST:
                create_ptr->guid = SPRD_GUI_LISTEDIT_ID;
                break;

            case GUIEDIT_TYPE_DATE:
                create_ptr->guid = SPRD_GUI_DATEEDIT_ID;
                break;

            case GUIEDIT_TYPE_TIME:
                create_ptr->guid = SPRD_GUI_TIMEEDIT_ID;
                break;

            case GUIEDIT_TYPE_IP:
                create_ptr->guid = SPRD_GUI_IPEDIT_ID;
                break;

            default:
                break;
            }
        }
    }
}

/*****************************************************************************/
// 	Description : get ctrl name
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC const char* MMITHEME_GetCtrlName(
                                        CAF_GUID_T  guid
                                        )
{
    uint32      i = 0;
    uint32      class_num = ARR_SIZE(s_ctrl_class_info);
    const char* name = PNULL;

    if ((SPRD_GUI_CTRL_GUID_STATR <= guid) &&
        (SPRD_GUI_CTRL_GUID_END >= guid))
    {
        for (i=0; i< class_num; i++)
        {
            if (guid == s_ctrl_class_info[i].guid)
            {
				name = s_ctrl_class_info[i].name;
                break;
            }
        }
    }

    return name;
}

/*****************************************************************************/
// 	Description : gui control query interface
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E VTLCTRL_QueryInterface(
                                           IGUICTRL_T   *iguictrl_ptr,
                                           CAF_GUID_T   guid,
                                           void         **object_pptr
                                           )
{
    CAF_RESULT_E    result = CAF_RESULT_FAILED;
    OBJECT_TYPE_T   *guictrl_ptr = (OBJECT_TYPE_T *)iguictrl_ptr;

    if ((IUNKNOWN_ID == guid) ||
        (guictrl_ptr->guid == guid))
    {
        *object_pptr = iguictrl_ptr;

        IUNKNOWN_AddRef(*object_pptr);

        result = CAF_RESULT_SUCCEEDED;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : gui control add ref
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC uint32 VTLCTRL_AddRef(
                             IGUICTRL_T     *iguictrl_ptr
                             )
{
    uint32          ref_num = 0;
    OBJECT_TYPE_T   *guictrl_ptr = (OBJECT_TYPE_T *)iguictrl_ptr;

    ref_num = ++guictrl_ptr->ref_num;

    return (ref_num);
}

/*****************************************************************************/
// 	Description : gui control release
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC uint32 VTLCTRL_Release(
                              IGUICTRL_T    *iguictrl_ptr
                              )
{
    uint32          ref_num = 0;
    OBJECT_TYPE_T   *guictrl_ptr = (OBJECT_TYPE_T *)iguictrl_ptr;

    // TODO
    SCI_ASSERT(0 != guictrl_ptr->type);     /*assert verified*/

    if (0 < --guictrl_ptr->ref_num)
    {
        ref_num = guictrl_ptr->ref_num;
    }
    else
    {
        TYPEMNG_Destroy(guictrl_ptr);
    }

    return (ref_num);
}

/*****************************************************************************/
//  Description : construct 
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN VTLCTRL_Construct(
                              IGUICTRL_T    *iguictrl_ptr,
                              void          *init_data_ptr
                              )
{
    OBJECT_TYPE_T   *guictrl_ptr = (OBJECT_TYPE_T *)iguictrl_ptr;

    return TYPEMNG_Init(guictrl_ptr, init_data_ptr);
}

/*****************************************************************************/
//  Description : destruct 
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN VTLCTRL_Destruct(
                              IGUICTRL_T    *iguictrl_ptr
                              )
{
    return TRUE;
}

/*****************************************************************************/
//  Description : handle msg 
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E VTLCTRL_HandleMsg(
                                      IGUICTRL_T            *iguictrl_ptr,
                                      CAF_MESSAGE_ID_E      msg_id,
                                      void*                 param_ptr
                                      )
{
    return VTLBASE_HandleMsg((CTRLBASE_OBJ_T*)iguictrl_ptr, msg_id, param_ptr);
}

/*****************************************************************************/
// 	Description : get gui control id
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC CAF_GUID_T VTLCTRL_GetCtrlGuid(
                                      IGUICTRL_T    *iguictrl_ptr
                                      )
{
    return BASE_GetCtrlGuid((CTRLBASE_OBJ_T*)iguictrl_ptr);
}

/*****************************************************************************/
// 	Description : set gui control handle
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void VTLCTRL_SetCtrlHandle(
                                  IGUICTRL_T    *iguictrl_ptr,
                                  CAF_HANDLE_T  ctrl_handle
                                  )
{
    BASE_SetCtrlHandle((CTRLBASE_OBJ_T*)iguictrl_ptr, ctrl_handle);
}

/*****************************************************************************/
// 	Description : get gui control handle
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC CAF_HANDLE_T VTLCTRL_GetCtrlHandle(
                                          IGUICTRL_T    *iguictrl_ptr
                                          )
{
    return BASE_GetCtrlHandle((CTRLBASE_OBJ_T*)iguictrl_ptr);
}

/*****************************************************************************/
// 	Description : set gui control state
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void VTLCTRL_SetState(
                             IGUICTRL_T         *iguictrl_ptr,
                             GUICTRL_STATE_T    state,
                             BOOLEAN            is_true
                             )
{
    CTRLBASE_OBJ_T* base_ctrl_ptr = (CTRLBASE_OBJ_T*)iguictrl_ptr;

    if (PNULL == iguictrl_ptr)
    {
        return;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    if (GUICTRL_STATE_SLIDE & state)
    {
        VTLBASE_SetSlideState(base_ctrl_ptr, is_true);
    }

    if (GUICTRL_STATE_DISABLE_ACTIVE & state)
    {
        VTLBASE_SetCanActive(base_ctrl_ptr, (BOOLEAN)!is_true);
    }

    if (GUICTRL_STATE_KEYMODE_ACTIVE & state)
    {
        VTLBASE_SetCanKbdActive(base_ctrl_ptr, (BOOLEAN)is_true);
    }

    if (GUICTRL_STATE_DISABLE_KEY & state)
    {
        VTLBASE_SetKbdEnabled(base_ctrl_ptr, (BOOLEAN)!is_true);
    }

    if (GUICTRL_STATE_DISABLE_TP & state)
    {
        VTLBASE_SetTpEnabled(base_ctrl_ptr, (BOOLEAN)!is_true);
    }

    if (GUICTRL_STATE_INVISIBLE & state)
    {
        VTLBASE_SetVisible(base_ctrl_ptr, (BOOLEAN)!is_true);
    }

    if (GUICTRL_STATE_TOPMOST & state)
    {
        if (is_true)
        {
            VTLBASE_SetTpPriority(base_ctrl_ptr, CTRL_TP_PRIORITY_HIGH);
        }
        else
        {
            VTLBASE_SetTpPriority(base_ctrl_ptr, CTRL_TP_PRIORITY_NORMAL);
        }
    }

    if (GUICTRL_STATE_TOPMOST_1 & state)
    {
        if (is_true)
        {
            VTLBASE_SetTpPriority(base_ctrl_ptr, CTRL_TP_PRIORITY_HIGHER);
        }
        else
        {
            VTLBASE_SetTpPriority(base_ctrl_ptr, CTRL_TP_PRIORITY_NORMAL);
        }
    }

    VTLBASE_OnStateChanged(base_ctrl_ptr, state, is_true);
}

/*****************************************************************************/
// 	Description : set gui control state
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetState(
                                IGUICTRL_T          *iguictrl_ptr,
                                GUICTRL_STATE_T     state
                                )
{
    BOOLEAN             result = TRUE;
    CTRLBASE_OBJ_T*     base_ctrl_ptr = (CTRLBASE_OBJ_T*)iguictrl_ptr;
    CTRL_TP_PRIORITY_E  tp_priority = 0;

    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    if (GUICTRL_STATE_SLIDE & state)
    {
        result = VTLBASE_GetSlideState(base_ctrl_ptr);
    }

    if (result && (GUICTRL_STATE_DISABLE_ACTIVE & state))
    {
        result = !VTLBASE_GetCanActive(base_ctrl_ptr);
    }

    if (result && (GUICTRL_STATE_KEYMODE_ACTIVE & state))
    {
        result = VTLBASE_GetCanKbdActive(base_ctrl_ptr);
    }

    if (result && (GUICTRL_STATE_DISABLE_KEY & state))
    {
        result = !VTLBASE_GetKbdEnabled(base_ctrl_ptr);
    }

    if (result && (GUICTRL_STATE_DISABLE_TP & state))
    {
        result = !VTLBASE_GetTpEnabled(base_ctrl_ptr);
    }

    if (result && (GUICTRL_STATE_INVISIBLE & state))
    {
        result = !VTLBASE_GetVisible(base_ctrl_ptr);
    }

    if (result && (GUICTRL_STATE_TOPMOST & state))
    {
        tp_priority = VTLBASE_GetTpPriority(base_ctrl_ptr);
        if (CTRL_TP_PRIORITY_HIGH == tp_priority)
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }

    if (result && (GUICTRL_STATE_TOPMOST_1 & state))
    {
        tp_priority = VTLBASE_GetTpPriority(base_ctrl_ptr);
        if (CTRL_TP_PRIORITY_HIGHER == tp_priority)
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : set gui control rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetRect(
                               IGUICTRL_T           *iguictrl_ptr,
                               const GUI_RECT_T     *rect_ptr
                               )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_SetRect((CTRLBASE_OBJ_T*)iguictrl_ptr, rect_ptr);
}

/*****************************************************************************/
// 	Description : get gui control rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetRect(
                               IGUICTRL_T   *iguictrl_ptr,
                               GUI_RECT_T   *rect_ptr
                               )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_GetRect((CTRLBASE_OBJ_T*)iguictrl_ptr, rect_ptr);
}

/*****************************************************************************/
// 	Description : set gui control lcd device
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void VTLCTRL_SetLcdDevInfo(
                                  IGUICTRL_T                *iguictrl_ptr,
                                  const GUI_LCD_DEV_INFO    *lcd_dev_ptr
                                  )
{
    if (PNULL == iguictrl_ptr)
    {
        return;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    VTLBASE_SetLcdDevInfo((CTRLBASE_OBJ_T*)iguictrl_ptr, lcd_dev_ptr);
}

/*****************************************************************************/
// 	Description : get gui control lcd device
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetLcdDevInfo(
                                     IGUICTRL_T         *iguictrl_ptr,
                                     GUI_LCD_DEV_INFO   *lcd_dev_ptr
                                     )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_GetLcdDevInfo((CTRLBASE_OBJ_T*)iguictrl_ptr, lcd_dev_ptr);
}

/*****************************************************************************/
// 	Description : set gui control properties
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetProperties(
                                     IGUICTRL_T                 *iguictrl_ptr,
                                     CAF_GUICTRL_PROPERTIES_T   properties,
                                     BOOLEAN                    is_true
                                     )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    // TODO:
    // CAF_GUICTRL_COM_PROPERTIES_ACTIVE与原来的代码不一样
    // 没有发MSG_CTL_PROPERTY_CHANGE消息
    VTLCTRL_SetState(iguictrl_ptr, properties, is_true);
    return TRUE;
}

/*****************************************************************************/
// 	Description : get gui control properties
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC CAF_GUICTRL_PROPERTIES_T VTLCTRL_GetProperties(
                                                      IGUICTRL_T    *iguictrl_ptr
                                                      )
{
    CTRLBASE_OBJ_T*     base_ctrl_ptr = (CTRLBASE_OBJ_T*)iguictrl_ptr;
    CAF_GUICTRL_PROPERTIES_T    properity = 0;

    if (PNULL == iguictrl_ptr)
    {
        return 0;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    if (VTLBASE_GetSlideState(base_ctrl_ptr))
    {
        properity |= GUICTRL_STATE_SLIDE;
    }

    if (!VTLBASE_GetCanActive(base_ctrl_ptr))
    {
        properity |= GUICTRL_STATE_DISABLE_ACTIVE;
    }

    if (VTLBASE_GetCanKbdActive(base_ctrl_ptr))
    {
        properity |= GUICTRL_STATE_KEYMODE_ACTIVE;
    }

    if (!VTLBASE_GetKbdEnabled(base_ctrl_ptr))
    {
        properity |= GUICTRL_STATE_DISABLE_KEY;
    }

    if (!VTLBASE_GetTpEnabled(base_ctrl_ptr))
    {
        properity |= GUICTRL_STATE_DISABLE_TP;
    }

    if (!VTLBASE_GetVisible(base_ctrl_ptr))
    {
        properity |= GUICTRL_STATE_INVISIBLE;
    }

    switch (VTLBASE_GetTpPriority(base_ctrl_ptr))
    {
    case CTRL_TP_PRIORITY_HIGH:
        properity |= GUICTRL_STATE_TOPMOST;
        break;

    case CTRL_TP_PRIORITY_HIGHER:
        properity |= GUICTRL_STATE_TOPMOST_1;
        break;

    case CTRL_TP_PRIORITY_NORMAL:
    default:
        break;
    }

    return properity;
}

/*****************************************************************************/
// 	Description : test gui control properties
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_TestProperties(
                                      IGUICTRL_T                *iguictrl_ptr,
                                      CAF_GUICTRL_PROPERTIES_T  properties
                                      )
{
    BOOLEAN         result = FALSE;
    CAF_GUICTRL_PROPERTIES_T    cur_property = 0;

    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    cur_property = VTLCTRL_GetProperties(iguictrl_ptr);
    if (properties == (cur_property & properties))
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : redraw gui control
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void VTLCTRL_Redraw(
                           IGUICTRL_T   *iguictrl_ptr
                           )
{
    if (PNULL == iguictrl_ptr)
    {
        return;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    BASE_Update((CTRLBASE_OBJ_T*)iguictrl_ptr);
}

/*****************************************************************************/
// 	Description : set gui control active
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetActive(
                                 IGUICTRL_T     *iguictrl_ptr,
                                 BOOLEAN        is_true
                                 )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_SetActive((CTRLBASE_OBJ_T*)iguictrl_ptr, is_true);
}

/*****************************************************************************/
// 	Description : is gui control active
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_IsActive(
                                IGUICTRL_T  *iguictrl_ptr
                                )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_GetActive((CTRLBASE_OBJ_T*)iguictrl_ptr);
}

/*****************************************************************************/
// 	Description : set control boder info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetBorder(
                                 IGUICTRL_T     *iguictrl_ptr,
                                 GUI_BORDER_T   *border_ptr
                                 )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_SetBorder((CTRLBASE_OBJ_T*)iguictrl_ptr, border_ptr);
}

/*****************************************************************************/
// 	Description : set control background info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetBg(
                             IGUICTRL_T     *iguictrl_ptr,
                             GUI_BG_T       *bg_ptr
                             )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_SetBg((CTRLBASE_OBJ_T*)iguictrl_ptr, bg_ptr);
}

/*****************************************************************************/
// 	Description : set control font info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetFont(
                               IGUICTRL_T       *iguictrl_ptr,
                               GUI_FONT_ALL_T   *font_ptr
                               )
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr =  PNULL;

    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASE_TYPE, (OBJECT_TYPE_PTR)iguictrl_ptr);
    return base_ctrl_vtbl_ptr->SetFont((CTRLBASE_OBJ_T*)iguictrl_ptr, font_ptr);
}

/*****************************************************************************/
// 	Description : get control font info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetFont(
                               IGUICTRL_T       *iguictrl_ptr,
                               GUI_FONT_ALL_T   *font_ptr
                               )
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr =  PNULL;

    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASE_TYPE, (OBJECT_TYPE_PTR)iguictrl_ptr);
    return base_ctrl_vtbl_ptr->GetFont((CTRLBASE_OBJ_T*)iguictrl_ptr, font_ptr);
}

/*****************************************************************************/
// 	Description : set control is display progress
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetProgress(
                                   IGUICTRL_T   *iguictrl_ptr,
                                   BOOLEAN      is_display
                                   )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_SetScrollbarEnabled((CTRLBASE_OBJ_T*)iguictrl_ptr, is_display);
}

/*****************************************************************************/
// 	Description : set control is circular handle up/down key
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetCircularHandleUpDown(
                                               IGUICTRL_T   *iguictrl_ptr,
                                               BOOLEAN      is_circular
                                               )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_SetUpDownLoop((CTRLBASE_OBJ_T*)iguictrl_ptr, is_circular);
}

/*****************************************************************************/
// 	Description : set control is circular handle left/right key
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetCircularHandleLeftRight(
                                                  IGUICTRL_T    *iguictrl_ptr,
                                                  BOOLEAN       is_circular
                                                  )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_SetLeftRightLoop((CTRLBASE_OBJ_T*)iguictrl_ptr, is_circular);
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetHeightByWidth(
                                        IGUICTRL_T  *iguictrl_ptr,  //in:
                                        uint16      width,          //in:
                                        uint16      *height_ptr     //in/out:
                                        )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_GetHeightByWidth((CTRLBASE_OBJ_T*)iguictrl_ptr, width, height_ptr);
}

/*****************************************************************************/
// 	Description : get control display rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetDisplayRect(
                                      IGUICTRL_T    *iguictrl_ptr,  //in:
                                      GUI_RECT_T    *rect_ptr       //in/out:
                                      )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_GetDisplayRect((CTRLBASE_OBJ_T*)iguictrl_ptr, rect_ptr);
}

/*****************************************************************************/
// 	Description : set control display rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetDisplayRect(
                                      IGUICTRL_T        *iguictrl_ptr,  //in:
                                      const GUI_RECT_T  *rect_ptr,      //in:
                                      BOOLEAN           is_update       //in:
                                      )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_SetDisplayRect((CTRLBASE_OBJ_T*)iguictrl_ptr, rect_ptr, is_update);
}

/*****************************************************************************/
// 	Description : set control select rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetSelectRect(
                                     IGUICTRL_T     *iguictrl_ptr,  //in:
                                     GUI_RECT_T     *rect_ptr       //in/out:
                                     )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_GetSelectRect((CTRLBASE_OBJ_T*)iguictrl_ptr, rect_ptr);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetSelectIndex(
                                      IGUICTRL_T    *iguictrl_ptr,  //in:
                                      BOOLEAN       *is_first_ptr,  //in/out:
                                      BOOLEAN       *is_last_ptr    //in/out:
                                      )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_GetSelectIndex((CTRLBASE_OBJ_T*)iguictrl_ptr, is_first_ptr, is_last_ptr);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetSelectIndex(
                                      IGUICTRL_T    *iguictrl_ptr,  //in:
                                      BOOLEAN       is_first,       //in:
                                      BOOLEAN       is_last         //in:
                                      )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_SetSelectIndex((CTRLBASE_OBJ_T*)iguictrl_ptr, is_first, is_last);
}

/*****************************************************************************/
// 	Description : get control both rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetBothRect(
                                   IGUICTRL_T       *iguictrl_ptr,  //in:
                                   GUI_BOTH_RECT_T  *both_rect_ptr  //in/out:
                                   )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_GetBothRect((CTRLBASE_OBJ_T*)iguictrl_ptr, both_rect_ptr);
}

/*****************************************************************************/
// 	Description : set control both rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetBothRect(
                                   IGUICTRL_T               *iguictrl_ptr,  //in:
                                   const GUI_BOTH_RECT_T    *both_rect_ptr  //in:
                                   )
{
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    SCI_ASSERT(((CTRLBASE_OBJ_T*)iguictrl_ptr)->object.type != 0);  /*assert verified*/

    return VTLBASE_SetBothRect((CTRLBASE_OBJ_T*)iguictrl_ptr, both_rect_ptr);
}

/*****************************************************************************/
// 	Description : notify parent ctrl
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetParentHandle(
                                   MMI_HANDLE_T ctrl_handle
                                   )
{
    MMI_HANDLE_T    parent_handle = 0;
    
    //句柄必须有效
    SCI_ASSERT(MMI_HANDLE_CONTROL == MMK_GetHandleType(ctrl_handle)); /*assert verified*/

    parent_handle = MMK_GetParentCtrlHandle(ctrl_handle);
    
    if (0 == parent_handle)
    {
        parent_handle = MMK_GetWinHandleByCtrl(ctrl_handle);
    }

    return (parent_handle);
}

/*****************************************************************************/
// 	Description : send notify msg to parent control or window
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMSG_SendNotify(
                                       MMI_HANDLE_T     ctrl_handle,
                                       MMI_MESSAGE_ID_E code
                                       )
{
    MMI_NOTIFY_T    notify = {0};
    
    notify.src_handle = ctrl_handle;
    notify.src_id     = MMK_GetCtrlId( ctrl_handle );
    notify.code       = code;
    
    return CTRLMSG_SendNotifyEx(ctrl_handle,code,&notify);
}

/*****************************************************************************/
// 	Description : send notify msg to parent control or window
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMSG_SendNotifyEx(
                                         MMI_HANDLE_T       ctrl_handle,
                                         MMI_MESSAGE_ID_E   code,
                                         DPARAM             notify_ptr
                                         )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    MMI_HANDLE_T    parent_handle = GetParentHandle(ctrl_handle);

    if ((0 != parent_handle) && 
        (MMK_SendMsg(parent_handle,code,notify_ptr)))
    {
        result = MMI_RESULT_TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : post notify msg to parent control or window
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMSG_PostNotify(
                                       MMI_HANDLE_T         ctrl_handle,
                                       MMI_MESSAGE_ID_E     code
                                       )
{
    MMI_NOTIFY_T    notify = {0};
    
    notify.src_handle = ctrl_handle;
    notify.src_id     = MMK_GetCtrlId( ctrl_handle );
    notify.code       = code;
    
    return CTRLMSG_PostNotifyEx(ctrl_handle,code,&notify,sizeof(MMI_NOTIFY_T));
}

/*****************************************************************************/
// 	Description : post notify msg to parent control or window
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLMSG_PostNotifyEx(
                                         MMI_HANDLE_T       ctrl_handle,
                                         MMI_MESSAGE_ID_E   code,
                                         DPARAM             notify_ptr, 
                                         uint32             size_of_notify
                                         )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    MMI_HANDLE_T    parent_handle = GetParentHandle(ctrl_handle);
    
    if ((0 != parent_handle) && 
        (MMK_PostMsg( parent_handle,code,notify_ptr,size_of_notify)))
    {
        result = MMI_RESULT_TRUE;
    }
    
    return (result);
}


