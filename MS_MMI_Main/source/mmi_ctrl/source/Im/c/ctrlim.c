/*****************************************************************************
** File Name:      ctrlim.c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmi_gui_trc.h"
#include "mmitheme_im.h"
#include "mmitheme_pos.h"
#include "guicommon.h"
#include "guictrl.h"
#include "guiwin.h"
#include "ctrlbase.h"

#include "ctrlim_base.h"
#include "ctrlim_export.h"
#include "ctrlim.h"
#include "mmk_window_internal.h"
#include "mmi_string.h"

/*
20101018 ����ʵ���������Ե�����
1 ����������������ָ���������Ե�ʱ�򣬲Ż�ȡĬ�ϵ�����
2 ����ָ�����������ʱ���л������в����л�����
*/

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*==============================================================================
Description: GetImPtr
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL CTRLIM_OBJ_T *GetImPtr(MMI_HANDLE_T im_handle);

/*==============================================================================
Description: ImConstruct
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �����ϱ�����Ӧ�ÿ��Է���FALSE������һ��false��ϵͳ�ͱ�����
==============================================================================*/
LOCAL BOOLEAN ImConstruct(
    CTRLBASE_OBJ_T      *iguictrl_ptr,
    CTRLIM_INIT_PARAM_T *im_param_ptr
);

/*==============================================================================
Description: ImDestruct
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN ImDestruct(CTRLBASE_OBJ_T *iguictrl_ptr);

/*==============================================================================
Description: ImHandleMsg
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL MMI_RESULT_E ImHandleMsg(
    CTRLBASE_OBJ_T *ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*==============================================================================
Description: ImSetRect
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �պ���!!! �˴����ܵ���ϵͳ��GUICTRL_SetRect, ����rect�ᱻ����
==============================================================================*/
LOCAL BOOLEAN ImSetRect(
    CTRLBASE_OBJ_T          *ctrl_ptr,
    const CAF_RECT_T    *rect_ptr
);

/*==============================================================================
Description: HandleGetFocus
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �����Ƿ��ؽ������뷨
!!! �ķ���ֵ������Ϣ�Ƿ���
==============================================================================*/
LOCAL BOOLEAN HandleGetFocus(CTRLIM_OBJ_T *im_ptr);

LOCAL void ResetLimit(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: �����õ�������������ת��Ϊ��ͨ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL GUIIM_LIMIT_T ConvertTypeToLimit(GUIIM_TYPE_T type_set);

/*==============================================================================
Description: ��english��abcת���ɶ�Ӧ�����ܺͶఴ������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ����Ҳת�����ܣ���˵�ǲ�����ַ��ŵ�
==============================================================================*/
LOCAL GUIIM_TYPE_T ConvertType(GUIIM_TYPE_T type);

/*==============================================================================
Description: �ӳ�ʼ��������ȡʵ����������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL GUIIM_LANGUAGE_T GetInputLang(GUIIM_INIT_PARAM_T *init_param_ptr);

/*==============================================================================
Description: ����Ƿ���Դӵ�ǰ�����뷽ʽ�л�����������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN CanSwitchToKeyboard(
    CTRLIM_OBJ_T *im_ptr,
    GUIIM_INPUT_TYPE_T *default_input
);

/*==============================================================================
Description: ����Ƿ���Դӵ�ǰ�����뷽ʽ�л�����������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN CanSwitchToTp(
    CTRLIM_OBJ_T *im_ptr,
    GUIIM_INPUT_TYPE_T *default_input
);

/*==============================================================================
Description: ��ʼ�����뷨
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN AppInit(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: �������뷨ʵ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void AppTerm(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: ���뷨�ؼ����Ƚ���Ϣ�������뷨Ӧ�ô������Ӵ�����������һ������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ����Ӧ���Ƿ����˸�ϵͳ��Ϣ
==============================================================================*/
LOCAL BOOLEAN AppHandle(CTRLIM_OBJ_T *im_ptr, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*==============================================================================
Description: ǿ��Ӧ��ˢ����ʾ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void AppDraw(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: �ύδ�ύ���ַ�
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ����ֵ��ʾ�Ƿ����ύ
==============================================================================*/
LOCAL BOOLEAN CommitLast(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: Ӧ�ø���method֮�󣬿ؼ�ͬ������ֵ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN ResetParameter(CTRLIM_OBJ_T *im_ptr, GUIIM_METHOD_T method);

/*==============================================================================
Description: �����뷨��������֮�����뷨�п��ܻᷢ���ı䡣
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
�������Եı���Ͳ�Ӱ����д����������
��Сд�ı��Ҳ��Ӱ�첻���ִ�Сд������
==============================================================================*/
LOCAL BOOLEAN SetParameter(CTRLIM_OBJ_T *im_ptr, GUIIM_METHOD_CFG_T *im_def_ptr);

//---------------
//set ϵ�к���
//---------------
//set parameter
//���ϵ�еĺ������ǻ�ȡ�µ����뷨��������
//ͨ�������ô��������������뷨�У����Ǵ��󣬱��������������ķ���
/*==============================================================================
Description: �����������뷨֮�������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetMethod(CTRLIM_OBJ_T *im_ptr, GUIIM_METHOD_T method);

/*==============================================================================
Description: SetLang
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetLang(CTRLIM_OBJ_T *im_ptr, GUIIM_LANGUAGE_T lang);

/*==============================================================================
Description: SetInputType
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetInput(CTRLIM_OBJ_T *im_ptr, GUIIM_INPUT_TYPE_T input);

/*==============================================================================
Description: SetType
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetType(CTRLIM_OBJ_T *im_ptr, GUIIM_TYPE_T type);

/*==============================================================================
Description: SetCap
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetCap(CTRLIM_OBJ_T *im_ptr, GUIIM_CAPITAL_MODE_T cap);

LOCAL BOOLEAN SwitchCap(CTRLIM_OBJ_T *im_ptr, GUIIM_CAPITAL_MODE_T cap);

/*==============================================================================
Description: ������������뷽ʽ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetInputSet(CTRLIM_OBJ_T *im_ptr, GUIIM_INPUT_TYPE_T input_set);

/*==============================================================================
Description: ����������������ͼ���
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetTypeSet(CTRLIM_OBJ_T *im_ptr, GUIIM_TYPE_T type_set);

/*==============================================================================
Description: ������Ӧ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SetCtrlRect(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: ������뷨�༭״̬�ı仯���������б仯��ʱ��ˢ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void CheckEditState(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: DisplaySoftkey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ���ص�ǰ�ı༭״̬��������ʾ�����Ϊ�༭״̬Ҳ��������������ʾ
==============================================================================*/
LOCAL BOOLEAN DisplaySoftkey(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: ͨ�����뷨�ռ��ʵ�����ݣ������뷨�ؼ��ĸ��ؼ��ύ�ַ���
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �򴴽����뷨�Ŀؼ��ύ���뷨Ӧ�õ��ύ��Ϣ
==============================================================================*/
LOCAL BOOLEAN CommitData(
    CTRLIM_OBJ_T *im_ptr,
    GUIIM_NOTIFY_PACK_T *pack_data_ptr
);

/*==============================================================================
Description: ��ѭ����Ϣ������ȡ��һ����Ϣ����������Ϣ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL GUIIM_NOTIFY_PACK_T * GetNotify(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: DealNotify
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void DealNotify(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: ����һ�����ư���
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �ڵ�����������һ���ڵ㣬����Ѿ�����ͬ�ģ���ȥ��֮ǰ��
==============================================================================*/
LOCAL void AddCustomKey(CTRLIM_OBJ_T *im_ptr, GUIIM_CUSTOM_KEY_PARAM_T* key_info_ptr);

/*==============================================================================
Description: ���ö�����Ϣ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetCustomKey(CTRLIM_OBJ_T *im_ptr, GUIIM_CUSTOM_KEY_PARAM_T *key_info_ptr);

/*==============================================================================
Description: �������еĶ�����Ϣ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SetAllCustomKey(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: ReleaseCustomKey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �ͷ��ڴ�
==============================================================================*/
LOCAL void ReleaseCustomKey(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: CloseAppendWin
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 
==============================================================================*/
LOCAL void CloseAppendWin(CTRLIM_OBJ_T *im_ptr);

LOCAL BOOLEAN IsSwitchMsg(MMI_MESSAGE_ID_E msg_id);

extern GUIIM_LANGUAGE_T MMIAPIIM_GetLanguage(void); /*lint -esym(526,MMIAPIIM_GetLanguage) */

/*****************************************************************************/
//  Description : init im class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ImCtrlInitVtbl(
    CTRLIM_VTBL_T        *im_vtbl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ImCtrlPackInitParam (
    GUIIM_INIT_PARAM_T     *im_init_ptr,   //in
    CTRLIM_INIT_PARAM_T   *im_param_ptr   //out
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ImTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ConvertInitParam (
    CTRLIM_INIT_PARAM_T   *im_param_ptr,   //in
    GUIIM_INIT_PARAM_T     *im_init_ptr    //out
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get im type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T IM_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "im",                                         // control name
                       sizeof (CTRLIM_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) ImConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) ImDestruct,      // control destructor function
                       sizeof (CTRLIM_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) ImCtrlPackInitParam,     // convert init param
                       sizeof (CTRLIM_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) ImCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init im class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ImCtrlInitVtbl (
    CTRLIM_VTBL_T        *im_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) im_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = ImHandleMsg;

    // ���½ӿ�����ʵ����Ƕform
    base_ctrl_vtbl_ptr->SetRect = ImSetRect;
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// �÷������ڽ��ؼ��Ķ����ʼ�������ṹת��Ϊ�ڲ��ĳ�ʼ�������ṹ
// ���ڿؼ��м̳й�ϵ���ؼ���ʼ��ʱ�ִ��в����������ڲ��ĳ�ʼ������Ҳ�����м̳й�ϵ
/*****************************************************************************/
LOCAL void ImCtrlPackInitParam (
    GUIIM_INIT_PARAM_T     *im_init_ptr,   //in
    CTRLIM_INIT_PARAM_T   *im_param_ptr   //out
)
{
    if (PNULL == im_init_ptr || PNULL == im_param_ptr)
    {
        return;
    }

    im_param_ptr->lang_id          = im_init_ptr->lang_id;
    im_param_ptr->input_type       = im_init_ptr->input_type;
    im_param_ptr->im_type          = im_init_ptr->im_type;
    im_param_ptr->cap_mode         = im_init_ptr->cap_mode;
    im_param_ptr->input_type_set   = im_init_ptr->input_type_set;
    im_param_ptr->im_type_set      = im_init_ptr->im_type_set;
    im_param_ptr->is_ban_symbol    = im_init_ptr->is_ban_symbol;
    im_param_ptr->tag              = im_init_ptr->tag;
    im_param_ptr->set_input_lang   = im_init_ptr->set_input_lang;
    im_param_ptr->f_get_context    = im_init_ptr->f_get_context;
    im_param_ptr->f_get_capacity   = im_init_ptr->f_get_capacity;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ImTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_IM_TYPE);
}

/*==============================================================================
Description: GetImPtr
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL CTRLIM_OBJ_T *GetImPtr(MMI_HANDLE_T im_handle)
{
    CTRLBASE_OBJ_T* ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr(im_handle);

    if (PNULL != ctrl_ptr)
    {
        //kevin.lou modified:delete assert
        //SCI_ASSERT(SPRD_GUI_IM_ID == IGUICTRL_GetCtrlGuid(ctrl_ptr)); /*assert verified*/
        if (SPRD_GUI_IM_ID != IGUICTRL_GetCtrlGuid((IGUICTRL_T*)ctrl_ptr))
        {
            ctrl_ptr = PNULL;
        }
    }

    return (CTRLIM_OBJ_T*)ctrl_ptr;
}

/*==============================================================================
Description: ImConstruct
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �����ϱ�����Ӧ�ÿ��Է���FALSE������һ��false��ϵͳ�ͱ�����
==============================================================================*/
LOCAL BOOLEAN ImConstruct(
    CTRLBASE_OBJ_T      *iguictrl_ptr,
    CTRLIM_INIT_PARAM_T *init_param_ptr
)
{
    CTRLIM_OBJ_T *im_ptr = (CTRLIM_OBJ_T *)iguictrl_ptr;
    GUIIM_METHOD_T default_method = GUIIM_M_NONE;
    BOOLEAN result = FALSE;
    GUIIM_METHOD_CFG_T *im_def_ptr = PNULL;
    GUIIM_LANGUAGE_T default_lang = GUIIM_LANG_NONE;

    if (PNULL == iguictrl_ptr || PNULL == init_param_ptr)
    {
        return FALSE;
    }

    im_ptr->win_handle = MMK_GetWinHandleByCtrl(iguictrl_ptr->handle);
    im_ptr->win_id = MMK_GetWinId(im_ptr->win_handle);

    ConvertInitParam(init_param_ptr, &im_ptr->init_param);

    //��ϵͳ֧�ֵ��������ͺ����뷽ʽ������������ǰ������
    im_ptr->method_param.input_set = init_param_ptr->input_type_set & MMITHEME_IMGetInputSet();
    im_ptr->method_param.type_set = init_param_ptr->im_type_set;
    im_ptr->method_param.tag = init_param_ptr->tag;
    ResetLimit(im_ptr);
    im_ptr->method_param.input_lang = GetInputLang(&im_ptr->init_param);

    if (GUIIM_LANG_NONE != im_ptr->method_param.input_lang)
    {
        default_lang = im_ptr->method_param.input_lang;
    }
    else
    {
        default_lang = MMITHEME_IMGetDefaultLang(im_ptr->method_param.limit);
    }

    //����Ĭ�����뷨
    //1 û������
    //2 û��ָ����������
    if (0 == im_ptr->method_param.limit
        && GUIIM_LANG_NONE == im_ptr->method_param.input_lang
        && MMITHEME_IMGetInputSet() == im_ptr->method_param.input_set)
    {
        default_method = MMITHEME_IMGetDefaultMethod();
        ResetParameter(im_ptr, default_method);

        im_ptr->is_default_method = 1;
    }
    else if (init_param_ptr->input_type & im_ptr->method_param.input_set)
    {
        im_ptr->method_param.im_set.lang = init_param_ptr->lang_id;
        im_ptr->method_param.im_set.input = init_param_ptr->input_type;
        im_ptr->method_param.im_set.type = ConvertType(init_param_ptr->im_type);
        im_ptr->method_param.im_set.capital = init_param_ptr->cap_mode;

        if (GUIIM_LANG_NONE == im_ptr->method_param.im_set.lang)
        {
            im_ptr->method_param.im_set.lang = default_lang;
        }

        im_def_ptr = MMITHEME_IMGetDefBySet(im_ptr->method_param.im_set.lang,
                                            im_ptr->method_param.im_set.input,
                                            im_ptr->method_param.im_set.type,
                                            im_ptr->method_param.im_set.capital);

        if (MMITHEME_IMIsNotLimit(im_ptr->method_param.input_lang, im_ptr->method_param.limit, im_def_ptr))
        {
            im_ptr->method_param.im_def_ptr = im_def_ptr;
        }

        if (PNULL != im_ptr->method_param.im_def_ptr)
        {
            im_ptr->method_param.im_set.method = im_ptr->method_param.im_def_ptr->method;
        }
    }

    //δ��ȡ���뷨����������ƺ��趨��ȡ���뷨
    if (PNULL == im_ptr->method_param.im_def_ptr)
    {
        GUIIM_INPUT_TYPE_T init_type_set = init_param_ptr->input_type;

        if (!(init_type_set & im_ptr->method_param.input_set))
        {
            init_type_set = im_ptr->method_param.input_set;
        }
        
        im_def_ptr = MMITHEME_IMGetDefByLimit(im_ptr->method_param.input_lang,
                                              init_type_set,
                                              init_param_ptr->cap_mode,
                                              im_ptr->method_param.limit);
        SCI_ASSERT(PNULL != im_def_ptr); /*assert verified*/
        ResetParameter(im_ptr, im_def_ptr->method);
    }

    //reset language
    if (GUIIM_LANG_NONE == im_ptr->method_param.im_set.lang)
    {
        im_ptr->method_param.im_set.lang = default_lang;
    }

    //init
    result = AppInit(im_ptr);

    IGUICTRL_SetState((IGUICTRL_T *)MMK_GetCtrlPtr(iguictrl_ptr->handle),GUICTRL_STATE_TOPMOST_1,TRUE);

    SCI_ASSERT(result); /*assert verified*/

    return result;
}



/*==============================================================================
Description: ImDestruct
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN ImDestruct(CTRLBASE_OBJ_T *iguictrl_ptr)
{
    CTRLIM_OBJ_T *im_ptr = (CTRLIM_OBJ_T *)iguictrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != iguictrl_ptr); /*assert verified*/
    if (PNULL == iguictrl_ptr)
    {
        return FALSE;
    }

    //�ͷŶ��ư�������Ϣ
    ReleaseCustomKey(im_ptr);

    //�����Ϣ����, ��Ҫ�Ǳ������뷨�ؼ�����֮������Ϣ��Ҫ����
    im_ptr->notify.pack_count = 0;

    return TRUE;
}


/*==============================================================================
Description: ImHandleMsg
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL MMI_RESULT_E ImHandleMsg(
    CTRLBASE_OBJ_T *ctrl_ptr,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    CTRLIM_OBJ_T *im_ptr = (CTRLIM_OBJ_T *)ctrl_ptr;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN result_app = FALSE;
    GUIIM_INPUT_TYPE_T default_input;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_IM_TYPE, (OBJECT_TYPE_PTR)ctrl_ptr);

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != ctrl_ptr); /*assert verified*/
    if (PNULL == ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //lost focus, ͳһ�����ύ
    if (MSG_CTL_LOSE_FOCUS == msg_id)
    {
        GUIIM_CommitLast(ctrl_ptr->handle);
    }
#ifdef TOUCH_PANEL_SUPPORT    
    else if (MSG_TP_PRESS_DOWN == msg_id || MSG_TP_PRESS_UP == msg_id )
    {
        if (im_ptr->is_msg_band) return MMI_RESULT_FALSE;
    }
#endif
    result_app = AppHandle(im_ptr, msg_id, param);

    if (result_app)
    {
        CheckEditState(im_ptr);
    }

    // �˴�����2��TRUE�ĸ����ʵ�Ƿ���ֵ�������ˣ�Ϊ�˲�����ϴ���޸ģ������
    if (2*TRUE == result_app)
    {
        result_app = FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_OPEN:
        break;

    case MSG_CTL_GET_FOCUS:
        if (MMITHEME_IMTpAllow())
        {
            GUIIM_LANGUAGE_T input_lang = MMIAPIIM_GetLanguage();
            
            //�п��ܴӸ��ര���������µ��������ԣ��������뷨�����ı�
            if (input_lang != im_ptr->method_param.im_def_ptr->lang
                && GUIIM_LANG_NONE != input_lang
                && GUIIM_LANG_NONE != im_ptr->method_param.im_def_ptr->lang)
            {
                //�������Է����ı�
                SetLang(im_ptr, input_lang);
            }
        }
        
        if (result_app) break;

        HandleGetFocus(im_ptr);
        
        break;

    case MSG_CTL_LOSE_FOCUS:
        //do nothing
        break;

    case MSG_CTL_GET_ACTIVE:
        //notify parent
        GUICTRL_SendNotify(ctrl_ptr->handle, MSG_NOTIFY_GET_ACTIVE);
        break;

    case MSG_CTL_LOSE_ACTIVE:
        if(CommitLast(im_ptr))
        {
            (void)(im_ptr->apis_ptr->handle)(im_ptr->im_handle, GUIIM_EVENT_SLEEP, PNULL);
            DealNotify(im_ptr);
        }

        //notify parent
        GUICTRL_SendNotify(ctrl_ptr->handle, MSG_NOTIFY_LOSE_ACTIVE);

        break;

    case MSG_CTL_PAINT:

        if (result_app) break;

        AppDraw(im_ptr);
        
        break;

    case MSG_CTL_CLOSE:
        
        if (im_ptr->is_default_method)
        {
            MMITHEME_ImSaveDefaultMethod(im_ptr->method_param.im_def_ptr->method);

			//after saved method to nv, then need to check whether current lang in nv
			//is matched with the method or not, if unmatch then to change lang in nv
			//e.g. save 'en' to nv, current lang in nv is 'none', we should store the 
			//corresponding lang 'english' of the method 'en' to nv, or 'en' won't be
			//the default method while constructing im in future
			if (GUIIM_LANG_NONE != im_ptr->method_param.im_def_ptr->lang
				&& MMIAPIIM_GetLanguage() != im_ptr->method_param.im_def_ptr->lang)
			{
				MMIAPIIM_SetLanguage(im_ptr->method_param.im_def_ptr->lang);
			}
        }

        //close the append win list
        CloseAppendWin(im_ptr);

        //term
        //AppTerm(im_ptr);
        if (PNULL != im_ptr->apis_ptr && PNULL != im_ptr->im_handle)
        {
            (im_ptr->apis_ptr->term)(im_ptr->im_handle);
        }

        im_ptr->im_handle = PNULL;

        break;

    case MSG_LCD_SWITCH:
        {
            GUI_RECT_T null_rect = {0};

            if (result_app)
            {
                SetCtrlRect(im_ptr);
            }
            else
            {
                //���Ӧ��δ������ǿ������ʵ�����ؽ�
                AppTerm(im_ptr);
                AppInit(im_ptr);
            }

            if (!GUI_EqualRect(ctrl_ptr->rect,null_rect))
            {
                GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_CHANGE_RECT, PNULL, 0);
            }
        }

        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:

        if (result_app) break;

        //͸�����ϲ㴦������ջ�����ύ��Ϣ
        SCI_MEMSET(&(im_ptr->last_commit), 0, sizeof(im_ptr->last_commit));
        result = MMI_RESULT_FALSE;
        break;

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:

        if (result_app) break;

        //�˴����ȴ������л������⡣
        //�ڱ༭�ؼ�֮��Ĳ��ֵ���ɻ�ý���������ؼ��������𽹵��л�
        if (im_ptr->method_param.im_def_ptr->method == GUIIM_M_HANDWRITING)     //��дʱ����Ҫ����˴�����Ϣ
        {
            MMI_HANDLE_T parent_handle = 0;
            GUI_RECT_T parent_rect = {0};
            GUI_POINT_T tp_point = {0};
            MMI_HANDLE_T win_handle = 0;
            MMI_HANDLE_T ctrl_handle = 0;
            BOOLEAN is_actived = FALSE;

            parent_handle = MMK_GetParentCtrlHandle(ctrl_ptr->handle);

            if (0 != parent_handle)
            {
                IGUICTRL_GetDisplayRect(MMK_GetCtrlPtr(parent_handle), &parent_rect);
            }
            else
            {
                parent_rect = MMITHEME_GetClientRect();
            }

            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);

            // 1 �����ڱ༭�ؼ���
            // 2 �����µĿؼ�������
            //������������
            // 1   ��������뷨
            // 2 ������

            if (!GUI_PointIsInRect(tp_point, parent_rect))
            {
                MMK_GetHandleByPos(tp_point, TRUE, &win_handle, &ctrl_handle, ctrl_ptr->handle);

                if ((0 != ctrl_handle) &&
                    (!MMK_IsAncestorCtrl(ctrl_handle,ctrl_ptr->handle)))
                {
                    is_actived = MMK_SetActiveCtrl(ctrl_handle, FALSE);

                    if (!is_actived)
                    {
                        //��send msg�ڼ䣬������Ϣ��Դ��
                        im_ptr->is_msg_band = TRUE;
                        MMK_SendMsg(ctrl_handle, MSG_TP_PRESS_DOWN, param);
                        MMK_SendMsg(ctrl_handle, MSG_TP_PRESS_UP, param);
                        im_ptr->is_msg_band = FALSE;
                    }

                    break;
                }
            }

            //����Ǵ������룬���л�����������
            //�ǵ��������д����theme����
            if (CanSwitchToTp(im_ptr, &default_input))
            {
                SetInput(im_ptr, default_input);
            }
            else        //�������룬�п�����Ҫ�л����
            {
                MMK_GetHandleByPos(tp_point, TRUE, &win_handle, &ctrl_handle, ctrl_ptr->handle);
                
                //��send msg�ڼ䣬������Ϣ��Դ��
                im_ptr->is_msg_band = TRUE;
                MMK_SendMsg(ctrl_handle, MSG_TP_PRESS_DOWN, param);
                MMK_SendMsg(ctrl_handle, MSG_TP_PRESS_UP, param);
                im_ptr->is_msg_band = FALSE;
            }
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

    default:

        if (result_app) break;

        //�ж��Ƿ���Ҫ�л�����������
        if (IsSwitchMsg(msg_id) && CanSwitchToKeyboard(im_ptr, &default_input))
        {
            BOOLEAN is_set = FALSE;
            
            /*NEWMS00180411 ��Ҫ�����NV��������д���뷨��Ȼ���tp->keyboard
            �ҵ�һ�������Ƶ����뷨������123���뷨����ʱ���ٵ���TP��
            ����ܵ�TP��123���뷨�������޷��л����⣬�޸ķ�����ֱ���л���
            �������뷨��ǰ������д����������
            */
            if (GUIIM_M_HANDWRITING == im_ptr->method_param.im_set.method)
            {
                im_ptr->method_param.im_set.type = GUIIM_TYPE_SMART;
            }

            is_set = SetInput(im_ptr, default_input);

            //ʹ�ܸü�����Ϊһ����Ч����
            if (is_set)
            {
                MMK_PostMsg(ctrl_ptr->handle, msg_id, param, sizeof(param));
            }
        }
        else
        {
            result = parent_vtbl_ptr->HandleEvent(ctrl_ptr, msg_id, param);
        }
        
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ConvertInitParam (
    CTRLIM_INIT_PARAM_T   *im_param_ptr,   //in
    GUIIM_INIT_PARAM_T     *im_init_ptr    //out
)
{
    if (PNULL == im_init_ptr || PNULL == im_param_ptr)
    {
        return;
    }

    im_init_ptr->lang_id          = im_param_ptr->lang_id;
    im_init_ptr->input_type       = im_param_ptr->input_type;
    im_init_ptr->im_type          = im_param_ptr->im_type;
    im_init_ptr->cap_mode         = im_param_ptr->cap_mode;
    im_init_ptr->input_type_set   = im_param_ptr->input_type_set;
    im_init_ptr->im_type_set      = im_param_ptr->im_type_set;
    im_init_ptr->is_ban_symbol    = im_param_ptr->is_ban_symbol;
    im_init_ptr->tag              = im_param_ptr->tag;
    im_init_ptr->set_input_lang   = im_param_ptr->set_input_lang;
    im_init_ptr->f_get_context    = im_param_ptr->f_get_context;
    im_init_ptr->f_get_capacity   = im_param_ptr->f_get_capacity;
}


/*==============================================================================
Description: ImSetRect
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �պ���!!! �˴����ܵ���ϵͳ��GUICTRL_SetRect, ����rect�ᱻ����
==============================================================================*/
LOCAL BOOLEAN ImSetRect(
    CTRLBASE_OBJ_T          *ctrl_ptr,
    const CAF_RECT_T    *rect_ptr
)
{
    return (PNULL != ctrl_ptr && PNULL != rect_ptr);
}


/*==============================================================================
Description: HandleGetFocus
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �����Ƿ��ؽ������뷨
!!! �ķ���ֵ������Ϣ�Ƿ���
==============================================================================*/
LOCAL BOOLEAN HandleGetFocus(CTRLIM_OBJ_T *im_ptr)
{
    BOOLEAN need_rebuild = FALSE;
    GUI_RECT_T rect_bak = {0};
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T *)im_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL != im_ptr)
    {
        // �Ƿ�����ʾ��С�ı仯
        rect_bak = ctrl_ptr->rect;
        SetCtrlRect(im_ptr);

        if (!GUI_EqualRect(ctrl_ptr->rect, rect_bak))
        {
            GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_CHANGE_RECT, PNULL, 0);
        }
    }

    return need_rebuild;
}


/*==============================================================================
Description: ����limit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: limit�ɶ����������õ���
==============================================================================*/
LOCAL void ResetLimit(CTRLIM_OBJ_T *im_ptr)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL != im_ptr)
    {
        im_ptr->method_param.limit = 0;

        im_ptr->method_param.limit |= ConvertTypeToLimit(im_ptr->method_param.type_set);

        if (im_ptr->init_param.is_ban_symbol)
        {
            im_ptr->method_param.limit |= GUIIM_LIMIT_SYMBOL;
        }
    }

    return;
}


/*==============================================================================
Description: �����õ�������������ת��Ϊ��ͨ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
    1 smart����ָ�ı�������ֻ������,�����֧��smart�����ֹlocal
      ��Ϊ����֧��ĳ��smart��֮���֧�ָĹ��������������룬����ƴ���ͱʻ�����ͬ
      ʱ֧�ֵ�
    2 �����֧��english & abc�����ֹenglish
      ��ΪӢ������ͬʱ֧��smart��abc��
==============================================================================*/
LOCAL GUIIM_LIMIT_T ConvertTypeToLimit(GUIIM_TYPE_T type_set)
{
    GUIIM_LIMIT_T limit = 0;

    //PDA�����뷨�л��߼���֮ǰ�Ĳ�ͬ������û�����һ����ȫ�İ취֮ǰ������˴���
    //zhoulianxiang 20110914
#if 0
#if defined(MMI_PDA_SUPPORT)
    if (!(GUIIM_TYPE_SMART & type_set))
#else
    if (!(GUIIM_TYPE_SMART & type_set) && !(GUIIM_TYPE_ENGLISH & type_set))
#endif
#else
    //NEWMS00186633 [6530][64x64_240x320TK_OP][������]�༭�������˻��û���ʱ��
    //ȡ��������������뷨�л���ťallow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;
    //���¿�����������
    if (!(GUIIM_TYPE_SMART & type_set))     
#endif
    {
        limit |= GUIIM_LIMIT_LOCAL;
    }

    if (!(GUIIM_TYPE_ENGLISH & type_set) && !(GUIIM_TYPE_ABC & type_set))
    {
        limit |= GUIIM_LIMIT_ENGLISH;
    }

    if (!(GUIIM_TYPE_DIGITAL & type_set))
    {
        limit |= GUIIM_LIMIT_DIGITAL;
    }

    return limit;
}


/*==============================================================================
Description: ��english��abcת���ɶ�Ӧ������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 
==============================================================================*/
LOCAL GUIIM_TYPE_T ConvertType(GUIIM_TYPE_T type)
{
    GUIIM_TYPE_T new_type = type;

    if (type == GUIIM_TYPE_ABC)
    {
        new_type = GUIIM_TYPE_MULTITAP;
    }
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU)    //���ܲ������������뷨
    else if (type == GUIIM_TYPE_ENGLISH)
    {
        new_type = GUIIM_TYPE_SMART;
    }
#endif
    
    return new_type;
}


/*==============================================================================
Description: �ӳ�ʼ��������ȡʵ����������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ��Ҫ����Ŀǰ��������� - english\abc\123 ͬʱ֧��
==============================================================================*/
LOCAL GUIIM_LANGUAGE_T GetInputLang(GUIIM_INIT_PARAM_T *init_param_ptr)
{
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != init_param_ptr); /*assert verified*/
    if (PNULL != init_param_ptr)
    {
        if (GUIIM_LANG_NONE != init_param_ptr->set_input_lang)
        {
            lang = init_param_ptr->set_input_lang;
        }
        else if (!(GUIIM_TYPE_SMART & init_param_ptr->im_type_set))
        {
            lang = GUIIM_LANG_ENGLISH;
        }
    }

    return lang;
}


/*==============================================================================
Description: ����Ƿ���Դӵ�ǰ�����뷽ʽ�л�����������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN CanSwitchToKeyboard(
    CTRLIM_OBJ_T *im_ptr,
    GUIIM_INPUT_TYPE_T *default_input
)
{
    if (PNULL == im_ptr) return FALSE;

    //��ǰ��������
    if ((GUIIM_INPUT_TYPE_KEYBOARD == im_ptr->method_param.im_set.input)
            || (GUIIM_INPUT_TYPE_QWERT == im_ptr->method_param.im_set.input))
    {
        return FALSE;
    }

    *default_input = MMITHEME_IMGetKbDefault(im_ptr->method_param.input_set);

    //�޼�������
    if (GUIIM_INPUT_TYPE_NONE == *default_input)
    {
        return FALSE;
    }

    return TRUE;
}


/*==============================================================================
Description: ����Ƿ���Դӵ�ǰ�����뷽ʽ�л�����������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN CanSwitchToTp(
    CTRLIM_OBJ_T *im_ptr,
    GUIIM_INPUT_TYPE_T *default_input
)
{
    if (PNULL == im_ptr) return FALSE;

    //��ǰ��������
    if ((GUIIM_INPUT_TYPE_TOUCHPEN == im_ptr->method_param.im_set.input)
            || (GUIIM_INPUT_TYPE_TP_QWERT == im_ptr->method_param.im_set.input)
            || (GUIIM_INPUT_TYPE_HANDWRITING == im_ptr->method_param.im_set.input))
    {
        return FALSE;
    }

    *default_input = MMITHEME_IMGetTpDefault(im_ptr->method_param.input_set);

    //�޼�������
    if (GUIIM_INPUT_TYPE_NONE == *default_input)
    {
        return FALSE;
    }

    return TRUE;
}



/*==============================================================================
Description: ��ʼ�����뷨
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN AppInit(CTRLIM_OBJ_T *im_ptr)
{
    GUIIM_INIT_KERNEL_T init_data = {0};
    BOOLEAN is_valid_method = FALSE;
    GUIIM_METHOD_T new_method = GUIIM_M_NONE;
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T *)im_ptr;

    SCI_ASSERT(PNULL != im_ptr); /*assert verified*/

    //������飬��ֹ�ڴ�й©
    SCI_ASSERT(PNULL == im_ptr->im_handle); /*assert verified*/
    //���õĲ�����飬���Ե������� method_param.im_def_ptr Ϊ׼
    SCI_ASSERT(PNULL != im_ptr->method_param.im_def_ptr); /*assert verified*/

    //������뷨�ĺϷ���
    //����Ƿ���������Ƿ���������뷨������һ��
    //���û��������뷨����assert
    is_valid_method = MMITHEME_IMCheckMethod(im_ptr->method_param.im_def_ptr,
                                             &new_method);

    if (!is_valid_method)
    {
        SCI_ASSERT(GUIIM_M_NONE != new_method); /*assert verified*/
        ResetParameter(im_ptr, new_method);
    }


    //�������»�ȡ���뷨api
    im_ptr->apis_ptr = MMITHEME_IMGetApis(im_ptr->method_param.im_def_ptr->method);
    //���û����Ӧ��api���壬��ôһ����ϵͳ���Ĵ�����
    SCI_ASSERT(PNULL != im_ptr->apis_ptr); /*assert verified*/

    //init data
    init_data.im_handle = ctrl_ptr->handle;
    init_data.win_handle = im_ptr->win_handle;
    init_data.win_id = im_ptr->win_handle;
    init_data.lcd_info_ptr = &ctrl_ptr->lcd_dev_info;
    init_data.method_param_ptr = &(im_ptr->method_param);
    init_data.f_get_context = im_ptr->init_param.f_get_context;
    init_data.f_get_capacity = im_ptr->init_param.f_get_capacity;

    im_ptr->im_handle = (im_ptr->apis_ptr->init)(&init_data);

    SCI_ASSERT(PNULL != im_ptr->im_handle); /*assert verified*/

    //��ʼ���ĺ�������
    SetAllCustomKey(im_ptr);
    SetCtrlRect(im_ptr);

    //AppDraw, ��Ҫˢ��һ�Σ���������ϵͳˢ�£��������ˢ���ӳ�
    if ( MMK_IsFocusWin( im_ptr->win_handle ))
    {
        AppDraw(im_ptr);
    }

    SCI_TRACE_LOW ("GUIIM AppInit im_ptr == %x method == %x", im_ptr, init_data.method_param_ptr->im_def_ptr->method);
    return TRUE;
}


/*==============================================================================
Description: �������뷨ʵ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void AppTerm(CTRLIM_OBJ_T *im_ptr)
{
    SCI_TRACE_LOW ("GUIIM AppTerm im_ptr == %x", im_ptr);
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL != im_ptr)
    {
        SCI_TRACE_LOW ("GUIIM AppTerm im_ptr == %x, method == %x", im_ptr, im_ptr->method_param.im_def_ptr->method);
        if (PNULL != im_ptr->apis_ptr && PNULL != im_ptr->im_handle)
        {
            (im_ptr->apis_ptr->term)(im_ptr->im_handle);
        }

        im_ptr->im_handle = PNULL;

        CommitLast(im_ptr);
    }

    return ;
}



/*==============================================================================
Description: ���뷨�ؼ����Ƚ���Ϣ�������뷨Ӧ�ô������Ӵ�����������һ������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ����Ӧ���Ƿ����˸�ϵͳ��Ϣ
==============================================================================*/
LOCAL BOOLEAN AppHandle(CTRLIM_OBJ_T *im_ptr, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    GUIIM_EVENT_DATA_U event_data = {0};
    BOOLEAN is_im_dealt_msg = FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    if (PNULL == im_ptr->im_handle || PNULL == im_ptr->apis_ptr)
    {
        return FALSE;
    }

    event_data.sys_msg.msg_id = msg_id;
    event_data.sys_msg.param = param;

	SCI_TRACE_LOW ("MMIIM:AppHandle method == %d, msg_id == %d, param == %d", im_ptr->method_param.im_def_ptr->method, msg_id, param);
    is_im_dealt_msg = (im_ptr->apis_ptr->handle)(im_ptr->im_handle,
                      GUIIM_EVENT_SYS_MSG, &event_data);

    if (!is_im_dealt_msg)
    {
        return FALSE;
    }

    DealNotify(im_ptr);

    return is_im_dealt_msg;
}


/*==============================================================================
Description: ǿ��Ӧ��ˢ����ʾ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void AppDraw(CTRLIM_OBJ_T *im_ptr)
{
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return;
    }

    if (PNULL == im_ptr->im_handle || PNULL == im_ptr->apis_ptr)
    {
        return;
    }

#if 0
    //��ǰ�������һ������������ڴ���ʱ��setactive_edit�����������뷨�����������ȴ����focus״̬
    if ( !MMK_IsFocusWin( im_ptr->win_handle ))
    {
        return;
    }
#endif

    (void)(im_ptr->apis_ptr->handle)(im_ptr->im_handle, GUIIM_EVENT_PAINT, PNULL);

    return;
}


/*==============================================================================
Description: �ύδ�ύ���ַ�
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ����ֵ��ʾ�Ƿ����ύ
==============================================================================*/
LOCAL BOOLEAN CommitLast(CTRLIM_OBJ_T *im_ptr)
{
	CTRLBASE_OBJ_T * ctrl_ptr = (CTRLBASE_OBJ_T *)im_ptr;
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    if (PNULL == im_ptr->im_handle || PNULL == im_ptr->apis_ptr)
    {
        return FALSE;
    }

    if (0 == im_ptr->last_commit.underline_len)
    {
        return FALSE;
    }

    //�ύδ�ύ���ַ�
    SCI_ASSERT(im_ptr->last_commit.underline_len <= GUIIM_COMMIT_BUFFER_MAX); /*assert verified*/
    im_ptr->last_commit.commit_len = im_ptr->last_commit.underline_len;

    MMIAPICOM_Wstrncpy(im_ptr->last_commit.commit,
                       im_ptr->last_commit.underline,
                       im_ptr->last_commit.commit_len);

    im_ptr->last_commit.commit[im_ptr->last_commit.commit_len] = '\0';
    im_ptr->last_commit.underline_len = 0;
    im_ptr->last_commit.underline[0] = '\0';

    GUICTRL_SendNotifyEx(ctrl_ptr->handle,
                         MSG_NOTIFY_IM_COMMIT,
                         &(im_ptr->last_commit));

    return TRUE;
}




/*==============================================================================
Description: Ӧ�ø���method֮�󣬿ؼ�ͬ������ֵ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN ResetParameter(CTRLIM_OBJ_T *im_ptr, GUIIM_METHOD_T method)
{
    GUIIM_METHOD_CFG_T *im_def_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if ((GUIIM_M_NONE == method) || (PNULL == im_ptr))
    {
        return FALSE;
    }

    if (PNULL != im_ptr->method_param.im_def_ptr
            && method == im_ptr->method_param.im_def_ptr->method)
    {
        return TRUE;
    }

    im_def_ptr = MMITHEME_IMGetDefByMethod(method);

    SCI_ASSERT(PNULL != im_def_ptr); /*assert verified*/

    //��ͬ�����뷨�ǲ�����ͬ�������õ�
    SCI_ASSERT(im_def_ptr != im_ptr->method_param.im_def_ptr); /*assert verified*/

    im_ptr->method_param.im_def_ptr = im_def_ptr;

    //����ǰ�������׼���Զ���ͬ��
    im_ptr->method_param.im_set.method = method;

    if (GUIIM_LANG_NONE != im_ptr->method_param.im_def_ptr->lang)
    {
        im_ptr->method_param.im_set.lang = im_ptr->method_param.im_def_ptr->lang;
    }

    if (GUIIM_TYPE_NONE != im_ptr->method_param.im_def_ptr->type)
    {
        im_ptr->method_param.im_set.type = im_ptr->method_param.im_def_ptr->type;
    }

    if (GUIIM_INPUT_TYPE_NONE != im_ptr->method_param.im_def_ptr->input)
    {
        im_ptr->method_param.im_set.input = im_ptr->method_param.im_def_ptr->input;
    }

    if (GUIIM_MODE_MAX != im_ptr->method_param.im_def_ptr->capital)
    {
        im_ptr->method_param.im_set.capital = im_ptr->method_param.im_def_ptr->capital;
    }

    return TRUE;
}




/*==============================================================================
Description: �����뷨��������֮�����뷨�п��ܻᷢ���ı䡣
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
�������Եı���Ͳ�Ӱ����д����������
��Сд�ı��Ҳ��Ӱ�첻���ִ�Сд������
==============================================================================*/
LOCAL BOOLEAN SetParameter(CTRLIM_OBJ_T *im_ptr, GUIIM_METHOD_CFG_T *im_def_ptr)
{
    BOOLEAN result_init = FALSE;
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T *)im_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != im_def_ptr); /*assert verified*/
    if ((PNULL == im_ptr)||(PNULL == im_def_ptr))
    {
        return FALSE;
    }
    
    //����֮ǰ�����뷨ʵ��
    AppTerm(im_ptr);

    im_ptr->method_param.im_def_ptr = im_def_ptr;
    im_ptr->method_param.im_set.method = im_def_ptr->method;

    //�ؽ����뷨ʵ��
    result_init = AppInit(im_ptr);

    if (result_init)
    {
        GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_SWICTH, PNULL, 0);
    }

    //ͨ�������ô��������������뷨�У����Ǵ��󣬱��������������ķ���
    SCI_ASSERT(result_init); /*assert verified*/

    return result_init;
}



//---------------
//set ϵ�к���
//---------------
//set parameter
//���ϵ�еĺ������ǻ�ȡ�µ����뷨��������
//ͨ�������ô��������������뷨�У����Ǵ��󣬱��������������ķ���

/*==============================================================================
Description: �����������뷨֮�������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetMethod(CTRLIM_OBJ_T *im_ptr, GUIIM_METHOD_T method)
{
    BOOLEAN result_init = FALSE;
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T *)im_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    //����֮ǰ�����뷨ʵ��
    AppTerm(im_ptr);

    ResetParameter(im_ptr, method);
    
    //�ؽ����뷨ʵ��
    result_init = AppInit(im_ptr);

    if (result_init)
    {
        GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_SWICTH, PNULL, 0);
    }

    //ͨ�������ô��������������뷨�У����Ǵ��󣬱��������������ķ���
    SCI_ASSERT(result_init); /*assert verified*/

    return result_init;
}



/*==============================================================================
Description: SetLang
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetLang(CTRLIM_OBJ_T *im_ptr, GUIIM_LANGUAGE_T lang)
{
    BOOLEAN result_reset = FALSE;
    GUIIM_METHOD_CFG_T *im_def_ptr = PNULL;
    GUIIM_APIS_PTR_T apis_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    if (im_ptr->method_param.im_set.lang == lang)
    {
        return TRUE;
    }

    //�Z��У�
    //1 local limit
    if (im_ptr->method_param.limit & GUIIM_LIMIT_LOCAL)
    {
        return FALSE;
    }    

    im_def_ptr = MMITHEME_IMGetDefBySet(lang,
                                        im_ptr->method_param.im_set.input,
                                        im_ptr->method_param.im_set.type,
                                        im_ptr->method_param.im_set.capital);

    if (PNULL == im_def_ptr)
    {
        GUIIM_INPUT_TYPE_T init_type_set = im_ptr->init_param.input_type;

        if (!(init_type_set & im_ptr->method_param.input_set))
        {
            init_type_set = im_ptr->method_param.input_set;
        }
        
        im_def_ptr = MMITHEME_IMGetDefByLimit(lang,
                                              init_type_set,
                                              im_ptr->method_param.im_set.capital,
                                              im_ptr->method_param.limit);

        if (PNULL == im_def_ptr)
        {
            //SCI_TRACE_LOW:"GUIIM: Setting wrong. lang = %#x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIM_1361_112_2_18_3_19_27_185,(uint8*)"d", lang);
            return FALSE;
        }

        //ResetParameter(im_ptr, im_def_ptr->method);
    }
    else
    {
        im_ptr->method_param.im_set.lang = lang;
    }

    apis_ptr = MMITHEME_IMGetApis(im_def_ptr->method);

    if (PNULL == apis_ptr)
    {
        //SCI_TRACE_LOW:"GUIIM: Setting wrong. lang = %#x. Undef method: %#x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIM_1370_112_2_18_3_19_27_186,(uint8*)"dd",lang, im_def_ptr->method);
        return FALSE;
    }
    
    //��д�������Ҫ������������ʼ������������дӦ���������Եġ�
    //��� - ������ - ����ƣ������뷨���������е�����
#if 0 
    if (im_def_ptr == im_ptr->method_param.im_def_ptr)
    {
        return TRUE;
    }
#endif

    result_reset = SetParameter(im_ptr, im_def_ptr);

    //������Щ����Ҳ��Ҫ�ٴθ��±��棬��ֹ���ֲ�һ�µ��������
    if (result_reset)
    {
        if (GUIIM_LANG_NONE != im_ptr->method_param.im_def_ptr->lang)
        {
            im_ptr->method_param.im_set.lang = im_ptr->method_param.im_def_ptr->lang;
        }

        if (GUIIM_TYPE_NONE != im_ptr->method_param.im_def_ptr->type)
        {
            im_ptr->method_param.im_set.type = im_ptr->method_param.im_def_ptr->type;
        }

        if (GUIIM_INPUT_TYPE_NONE != im_ptr->method_param.im_def_ptr->input)
        {
            im_ptr->method_param.im_set.input = im_ptr->method_param.im_def_ptr->input;
        }

        if (GUIIM_MODE_MAX != im_ptr->method_param.im_def_ptr->capital)
        {
            im_ptr->method_param.im_set.capital = im_ptr->method_param.im_def_ptr->capital;
        }
    }
    
    return result_reset;
}


/*==============================================================================
Description: SetInputType
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetInput(CTRLIM_OBJ_T *im_ptr, GUIIM_INPUT_TYPE_T input)
{
    BOOLEAN result_reset = FALSE;
    GUIIM_METHOD_CFG_T *im_def_ptr = PNULL;
    GUIIM_APIS_PTR_T apis_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return FALSE;
    }
    
    if (im_ptr->method_param.im_set.input == input)
    {
        return TRUE;
    }

    //check, if the input is allowed
    //...
    if (!(im_ptr->method_param.input_set & input))
    {
        //SCI_TRACE_LOW:"GUIIM: Setting wrong, not in set. input = %#x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIM_1418_112_2_18_3_19_27_187,(uint8*)"d", input);
        return FALSE;
    }

    im_def_ptr = MMITHEME_IMGetDefBySet(im_ptr->method_param.im_set.lang,
                                        input,
                                        im_ptr->method_param.im_set.type,
                                        im_ptr->method_param.im_set.capital);

    if (PNULL == im_def_ptr)
    {
        //SCI_TRACE_LOW:"GUIIM: Setting wrong. input = %#x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIM_1429_112_2_18_3_19_27_188,(uint8*)"d", input);
        return FALSE;
    }

    apis_ptr = MMITHEME_IMGetApis(im_def_ptr->method);

    if (PNULL == apis_ptr)
    {
        BOOLEAN can_get_9or26 = FALSE;
        GUIIM_INPUT_TYPE_T new_input = GUIIM_INPUT_TYPE_NONE;
        GUIIM_METHOD_CFG_T *im_cfg_ptr = PNULL;
        
        //SCI_TRACE_LOW:"GUIIM: Can not get api. input = %#x. Undef method: %#x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIM_1442_112_2_18_3_19_27_189,(uint8*)"dd",input, im_def_ptr->method);

        can_get_9or26 = MMITHEME_IMCheck9Or26Input(&im_ptr->method_param.im_set,
                                                   input,
                                                   &new_input,
                                                   &im_cfg_ptr);

        if(can_get_9or26)
        {
            input = new_input;
            im_def_ptr = im_cfg_ptr;
        }
        else
        {
            return FALSE;
        }
    }

    im_ptr->method_param.im_set.input = input;

    if (im_def_ptr == im_ptr->method_param.im_def_ptr)
    {
        return TRUE;
    }

    result_reset = SetParameter(im_ptr, im_def_ptr);

    return result_reset;
}


/*==============================================================================
Description: SetType
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetType(CTRLIM_OBJ_T *im_ptr, GUIIM_TYPE_T type)
{
    BOOLEAN result_reset = FALSE;
    GUIIM_METHOD_CFG_T *im_def_ptr = PNULL;
    GUIIM_APIS_PTR_T apis_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    type = ConvertType(type);

    if (im_ptr->method_param.im_set.type == type)
    {
        return TRUE;
    }

    //check, if the type is allowed
    //...
    if (!(im_ptr->method_param.type_set & type))
    {
        //SCI_TRACE_LOW:"GUIIM: Setting wrong, not in set. type = %#x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIM_1502_112_2_18_3_19_27_190,(uint8*)"d", type);
        return FALSE;
    }

    im_def_ptr = MMITHEME_IMGetDefBySet(im_ptr->method_param.im_set.lang,
                                        im_ptr->method_param.im_set.input,
                                        type,
                                        im_ptr->method_param.im_set.capital);

    if (PNULL == im_def_ptr)
    {
        //SCI_TRACE_LOW:"GUIIM: Setting wrong. type = %#x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIM_1513_112_2_18_3_19_27_191,(uint8*)"d", type);
        return FALSE;
    }

    apis_ptr = MMITHEME_IMGetApis(im_def_ptr->method);

    if (PNULL == apis_ptr)
    {
        //SCI_TRACE_LOW:"GUIIM: Setting wrong. type = %#x. Undef method: %#x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIM_1522_112_2_18_3_19_27_192,(uint8*)"dd",type, im_def_ptr->method);
        return FALSE;
    }

    im_ptr->method_param.im_set.type = type;

    if (im_def_ptr == im_ptr->method_param.im_def_ptr)
    {
        return TRUE;
    }

    result_reset = SetParameter(im_ptr, im_def_ptr);

    return result_reset;
}


/*==============================================================================
Description: SetCap
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetCap(CTRLIM_OBJ_T *im_ptr, GUIIM_CAPITAL_MODE_T cap)
{
    BOOLEAN result_reset = FALSE;
    GUIIM_METHOD_CFG_T *im_def_ptr = PNULL;
    GUIIM_APIS_PTR_T apis_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    if (im_ptr->method_param.im_set.capital == cap)
    {
        return TRUE;
    }

    im_def_ptr = MMITHEME_IMGetDefBySet(im_ptr->method_param.im_set.lang,
                                        im_ptr->method_param.im_set.input,
                                        im_ptr->method_param.im_set.type,
                                        cap);

    if (PNULL == im_def_ptr)
    {
        //SCI_TRACE_LOW:"GUIIM: Setting wrong. cap = %#x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIM_1569_112_2_18_3_19_27_193,(uint8*)"d", cap);
        return FALSE;
    }

    apis_ptr = MMITHEME_IMGetApis(im_def_ptr->method);

    if (PNULL == apis_ptr)
    {
        //SCI_TRACE_LOW:"GUIIM: Setting wrong. cap = %#x. Undef method: %#x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIM_1578_112_2_18_3_19_27_194,(uint8*)"dd",cap, im_def_ptr->method);
        return FALSE;
    }

    im_ptr->method_param.im_set.capital = cap;

    if (im_def_ptr == im_ptr->method_param.im_def_ptr)
    {
        return TRUE;
    }

    result_reset = SetParameter(im_ptr, im_def_ptr);

    return result_reset;
}


/*==============================================================================
Description: SwitchCap
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SwitchCap(CTRLIM_OBJ_T *im_ptr, GUIIM_CAPITAL_MODE_T cap)
{
    GUIIM_METHOD_CFG_T *im_def_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    if (im_ptr->method_param.im_set.capital == cap)
    {
        return TRUE;
    }

    im_def_ptr = MMITHEME_IMGetDefBySet(im_ptr->method_param.im_set.lang,
                                        im_ptr->method_param.im_set.input,
                                        im_ptr->method_param.im_set.type,
                                        cap);

    if (PNULL == im_def_ptr)
    {
        //SCI_TRACE_LOW:"GUIIM: Setting wrong. cap = %#x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIIM_1623_112_2_18_3_19_27_195,(uint8*)"d", cap);
        return FALSE;
    }

    im_ptr->method_param.im_set.capital = cap;
    im_ptr->method_param.im_def_ptr = im_def_ptr;

    return TRUE;
}


/*==============================================================================
Description: ������������뷽ʽ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetInputSet(CTRLIM_OBJ_T *im_ptr, GUIIM_INPUT_TYPE_T input_set)
{
    GUIIM_INPUT_TYPE_T modified_set = MMITHEME_IMGetInputSet();
    GUIIM_METHOD_CFG_T *im_def_ptr = PNULL;
    BOOLEAN result = FALSE;
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T *)im_ptr;
    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return FALSE;
    }
    

    if (input_set == im_ptr->init_param.input_type_set)
    {
        return TRUE;
    }

    im_ptr->init_param.input_type_set = input_set;

    modified_set &= input_set;

    if (modified_set == im_ptr->method_param.input_set)
    {
        return TRUE;
    }

    im_ptr->method_param.input_set = modified_set;

    if (im_ptr->method_param.im_set.input & modified_set)
    {
        return TRUE;
    }

    //���趨�ķ�ʽ��������ǰ�ķ�ʽ�����ȡ��ǰ�����µ�Ĭ�Ϸ�ʽ
    im_def_ptr = MMITHEME_IMGetDefByLimit(im_ptr->method_param.input_lang,
                                          im_ptr->method_param.input_set,
                                          im_ptr->init_param.cap_mode,
                                          im_ptr->method_param.limit);
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_def_ptr); /*assert verified*/
    if (PNULL == im_def_ptr)
    {
        return FALSE;
    }
    
    ResetParameter(im_ptr, im_def_ptr->method);

    //term first, if need
    AppTerm(im_ptr);
    //init
    result = AppInit(im_ptr);

    if (result)
    {
        GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_SWICTH, PNULL, 0);
    }

    SCI_ASSERT(result); /*assert verified*/

    return result;
}


/*==============================================================================
Description: ����������������ͼ���
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetTypeSet(CTRLIM_OBJ_T *im_ptr, GUIIM_TYPE_T type_set)
{
    GUIIM_LIMIT_T limit = 0;
    GUIIM_METHOD_CFG_T *im_def_ptr = PNULL;
    BOOLEAN result = FALSE;
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T *)im_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return FALSE;
    }
    
    limit = ConvertTypeToLimit(type_set);

    if (limit == im_ptr->method_param.limit)
    {
        return TRUE;
    }

    ResetLimit(im_ptr);

    im_ptr->init_param.im_type_set = type_set;
    im_ptr->method_param.type_set = type_set;

    if (MMITHEME_IMIsNotLimit(im_ptr->method_param.input_lang, limit, im_ptr->method_param.im_def_ptr))
    {
        return TRUE;
    }

    //���趨�ķ�ʽ��������ǰ�ķ�ʽ�����ȡ��ǰ�����µ�Ĭ�Ϸ�ʽ
    im_def_ptr = MMITHEME_IMGetDefByLimit(im_ptr->method_param.input_lang,
                                          im_ptr->method_param.input_set,
                                          im_ptr->init_param.cap_mode,
                                          im_ptr->method_param.limit);
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_def_ptr); /*assert verified*/
    if (PNULL == im_def_ptr)
    {
        return FALSE;
    }

    ResetParameter(im_ptr, im_def_ptr->method);

    //term first, if need
    AppTerm(im_ptr);
    //init
    result = AppInit(im_ptr);

    if (result)
    {
        GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_SWICTH, PNULL, 0);
    }

    SCI_ASSERT(result); /*assert verified*/

    return result;
}




/*==============================================================================
Description: ������Ӧ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SetCtrlRect(CTRLIM_OBJ_T *im_ptr)
{
    GUIIM_EVENT_DATA_U data = {0};
    BOOLEAN is_got = FALSE;
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T *)im_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return;
    }
    
    //δ��ʼ�����򲻴���
    if (PNULL == im_ptr->im_handle || PNULL == im_ptr->apis_ptr)
    {
        return;
    }
    

    SCI_MEMSET(&(ctrl_ptr->rect), 0, sizeof(ctrl_ptr->rect));

//NEWMS00196109 [6530][64X64_320X240TK_QW]��ѡ���ַ���֧�� ���ʵ������
//��֧�ִ������룬�����뷨�ؼ�����Ϊ0�����µ����ѡ������ʱ����Ϣ�޷��������뷨�ؼ����޷�����
#if 0
    //�޴������룬��������
    //һЩ�����Ӧ��ͨ�������޴�������ֹ���뷨��ȡ������Ϣ
    if (!(GUIIM_INPUT_TYPE_TOUCHPEN & im_ptr->method_param.input_set)
            && !(GUIIM_INPUT_TYPE_TP_QWERT & im_ptr->method_param.input_set)
            && !(GUIIM_INPUT_TYPE_HANDWRITING & im_ptr->method_param.input_set))
    {
        return;
    }
#endif

    //ֱ�Ӵ����뷨Ӧ�û�ȡ��������
    is_got = (im_ptr->apis_ptr->handle)(im_ptr->im_handle,
                                        GUIIM_EVENT_GET_CTRL_RECT, &data);

    if (is_got) ctrl_ptr->rect = data.get_ctrl_rect;


    return;
}



/*==============================================================================
Description: ������뷨�༭״̬�ı仯���������б仯��ʱ��ˢ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void CheckEditState(CTRLIM_OBJ_T *im_ptr)
{
    BOOLEAN is_im_displayed = FALSE;
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T *)im_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return;
    }
    
    if (PNULL == im_ptr->im_handle || PNULL == im_ptr->apis_ptr)
    {
        return ;
    }

     is_im_displayed = DisplaySoftkey(im_ptr);

    //����Ǳ༭״̬ת���Ǳ༭״̬����֪ͨ�ϲ�ؼ�ˢ��
    if (!is_im_displayed)
    {
        GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_REFRESH_SOFTKEY, PNULL, 0);
    }


    return;
}


/*==============================================================================
Description: DisplaySoftkey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �����Ƿ�ؼ��Ѿ�������softkey�����Ƿ���Ҫ֪ͨ���ؼ�ˢ��
ֻ�е��ӱ༭״̬�仯���Ǳ༭״̬��ʱ�򣬲���Ҫ�༭�ؼ�ˢ��softkey
1 �ַ�����״̬
ȷ�� x ɾ��
2 �ַ���ѡ����С��ǩ״̬
ȷ�� x ����
3 �ַ���ѡ����С��ǩ״̬
ȷ�� x ȡ��
4 ��д״̬
���ֲ���
==============================================================================*/
LOCAL BOOLEAN DisplaySoftkey(CTRLIM_OBJ_T *im_ptr)
{
    BOOLEAN             handle_result = FALSE;
    MMI_TEXT_ID_T       text_left = 0;
    MMI_TEXT_ID_T       text_middle = 0;
    MMI_TEXT_ID_T       text_right = 0;
    GUIIM_EVENT_DATA_U  data = {0};
    BOOLEAN             is_not_editing = FALSE;
    BOOLEAN             is_state_change = FALSE;
    BOOLEAN             need_display = FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return FALSE;
    }
    
    //��������ɶҲ����
    if (PNULL == im_ptr->im_handle || PNULL == im_ptr->apis_ptr)
    {
        return FALSE;
    }

    handle_result = (im_ptr->apis_ptr->handle)(im_ptr->im_handle, GUIIM_EVENT_GET_EDIT_STATE, &data);

    //��ȡʧ�ܣ������Ǳ༭״̬
    if (!handle_result)
    {
        data.edit_state = GUIIM_EDIT_NONE;
    }

    is_not_editing = (GUIIM_EDIT_NONE == data.edit_state);
    is_state_change = (data.edit_state != im_ptr->edit_state_bak);

    //�༭״̬δ�仯����ˢ��
    if (!is_state_change)
    {
        return TRUE;
    }

    im_ptr->edit_state_bak = data.edit_state;

    //�Ǳ༭״̬
    //״̬�仯����Ҫ�󸸿ؼ�ˢ��softkey - ���ˣ��϶���״̬�仯����
    if (is_not_editing)
    {
        return FALSE;
    }

    need_display = MMITHEME_IMGetSoftKeyText(data.edit_state,
                                             &text_left,
                                             &text_middle,
                                             &text_right);

    if (need_display)
    {
//begin 20101202
//#ifdef MAINLCD_SIZE_128X64
#if defined MAINLCD_SIZE_128X64 || defined GUIF_SOFTKEY_PDASTYLE
#else
        GUIWIN_SetSoftkeyTextId(MMK_GetWinHandleByCtrl(((CTRLBASE_OBJ_T*)im_ptr)->handle),
                                text_left, text_middle, text_right, TRUE);
#endif
//end 20101202
    }
 
    return TRUE;
}


/*==============================================================================
Description: ͨ�����뷨�ռ��ʵ�����ݣ������뷨�ؼ��ĸ��ؼ��ύ�ַ���
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: �򴴽����뷨�Ŀؼ��ύ���뷨Ӧ�õ��ύ��Ϣ
==============================================================================*/
LOCAL BOOLEAN CommitData(
    CTRLIM_OBJ_T *im_ptr,
    GUIIM_NOTIFY_PACK_T *pack_data_ptr
)
{
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T *)im_ptr;

    if (PNULL == im_ptr || PNULL == pack_data_ptr)
    {
        return FALSE;
    }

    switch (pack_data_ptr->notify)
    {
    case GUIIM_NOTIFY_COMMIT:
        im_ptr->last_commit = pack_data_ptr->data.commit_data;

        //�����ύ��Ϣ
        if ('\b' == pack_data_ptr->data.commit_data.commit[0])
        {
            GUICTRL_SendNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_BACKSPACE, PNULL);
        }
        else
        {
#ifdef GUIF_IM_NEWLINE_ENTER
            if (0x0d == pack_data_ptr->data.commit_data.commit[0] &&
                0x0a == pack_data_ptr->data.commit_data.commit[1] &&
                2 == pack_data_ptr->data.commit_data.commit_len)
            {
                im_ptr->last_commit.commit[0] = 0x0a;
                im_ptr->last_commit.commit_len = 1;
            }
#endif
            GUICTRL_SendNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_COMMIT, &(im_ptr->last_commit));
        }

        break;

	case GUIIM_NOTIFY_DELETE:
		GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_BACKSPACE, PNULL, 0);
		break;
    case GUIIM_NOTIFY_LONG_DEL:
        GUICTRL_SendNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_LONG_DEL, PNULL);
        break;

    case GUIIM_NOTIFY_CHANGE_RECT:
        SetCtrlRect(im_ptr);

        //�л���ȫ����дʱ����ʾ�����С��һ�£�postˢ�½��������¿�������Ĵ���
        //��ͬ����Ϣ���ܴ��ڷ��գ����ú��������
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
        GUICTRL_SendNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_CHANGE_RECT, PNULL); 
#else
        GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_CHANGE_RECT, PNULL, 0);
#endif     
        break;

    case GUIIM_NOTIFY_CHANGE_EDIT_STATE:
        GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_REFRESH_SOFTKEY, PNULL, 0);
        break;

    case GUIIM_NOTIFY_SWITCH_METHOD:
        ResetParameter(im_ptr, pack_data_ptr->data.switch_method);
        SetCtrlRect(im_ptr);
        SetAllCustomKey(im_ptr);
        GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_SWICTH, PNULL, 0);
        break;

    case GUIIM_NOTIFY_SWITCH_CAPITAL:
        SwitchCap(im_ptr, pack_data_ptr->data.switch_capital);
        break;

    case GUIIM_NOTIFY_SET_METHOD:
        SetMethod(im_ptr, pack_data_ptr->data.set_method);

        //�л���ȫ����дʱ����ʾ�����С��һ��
        //������鲻�ܽ������뷨Ӧ���Լ�������Ϊ���뷨Ӧ���ǲ�֪���Լ���������������Ӧ������һ��
        //������뷨Ӧ�ò���������Ӧ�ã�ÿ�ε���һ��MSG_NOTIFY_IM_CHANGE_RECT����ᵼ�����״ν������뷨ʱ��
        //���뷨Ӧ���������һ��δ�������Ϣ��MSG_NOTIFY_IM_CHANGE_RECT
        SetCtrlRect(im_ptr);
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
        GUICTRL_SendNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_CHANGE_RECT, PNULL); 
#else
        GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_CHANGE_RECT, PNULL, 0);
#endif     
        break;

    case GUIIM_NOTIFY_SET_LANG:
        SetLang(im_ptr, pack_data_ptr->data.set_lang);
        break;

    case GUIIM_NOTIFY_SET_TYPE:
        SetType(im_ptr, pack_data_ptr->data.set_type);
        break;

    case GUIIM_NOTIFY_SET_INPUT:
        SetInput(im_ptr, pack_data_ptr->data.set_input);
        break;

    case GUIIM_NOTIFY_SET_CAPITAL:
        SetCap(im_ptr, pack_data_ptr->data.set_capital);
        break;

    case GUIIM_NOTIFY_PAINT:
        GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_PAINT, PNULL, 0);
        AppDraw(im_ptr);
        break;

    case GUIIM_NOTIFY_HIDE:
        {
            GUIIM_INPUT_TYPE_T default_input = GUIIM_M_NONE;

            if (CanSwitchToKeyboard(im_ptr, &default_input))
            {
                /*NEWMS00180411 ��Ҫ�����NV��������д���뷨��Ȼ���tp->keyboard
                �ҵ�һ�������Ƶ����뷨������123���뷨����ʱ���ٵ���TP��
                ����ܵ�TP��123���뷨�������޷��л����⣬�޸ķ�����ֱ���л���
                �������뷨��ǰ������д����������
                */
                if (GUIIM_M_HANDWRITING == im_ptr->method_param.im_set.method)
                {
                    im_ptr->method_param.im_set.type = GUIIM_TYPE_SMART;
                }
                
                SetInput(im_ptr, default_input);
            }
            else
            {
                GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_HIDE, PNULL, 0);
            }
        }
        
        break;

    case GUIIM_NOTIFY_TRANS_MSG:
        GUICTRL_SendNotifyEx(ctrl_ptr->handle, pack_data_ptr->data.trans_msg.msg_id, 
            pack_data_ptr->data.trans_msg.param);
        break;

    case GUIIM_NOTIFY_SETCURSOR:
        GUICTRL_SendNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_SETCURSOR,
            MAKE_CURSOR_PARAM(pack_data_ptr->data.setcursor_msg.type, pack_data_ptr->data.setcursor_msg.value));
        break;

    case GUIIM_NOTIFY_GETCURSOR:
        GUICTRL_SendNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_GETCURSOR,
            (DPARAM)&pack_data_ptr->data.getcursor_value);
        break;

    case GUIIM_NOTIFY_SET_HIGHLIGHT:
        GUICTRL_SendNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_SET_HIGHLIGHT,
            MAKE_HIGHLIGHT_PARAM(pack_data_ptr->data.set_highlight_msg.start_index,
                pack_data_ptr->data.set_highlight_msg.end_index));
        break;

    case GUIIM_NOTIFY_COMMIT_UNDERLINE:
        CommitLast(ctrl_ptr);
        break;

    case GUIIM_NOTIFY_AUTO_SWITCH_METHOD:
        {
            BOOLEAN result_init = FALSE;
            
            if (PNULL != im_ptr->apis_ptr && PNULL != im_ptr->im_handle)
            {
                (im_ptr->apis_ptr->term)(im_ptr->im_handle);
            }

            im_ptr->im_handle = PNULL;
            
            ResetParameter(ctrl_ptr, pack_data_ptr->data.set_method);
            
            result_init = AppInit(ctrl_ptr);

            if (result_init)
            {
                GUICTRL_SendNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_SWICTH, PNULL);
            }
        }
        break;

    default:
        break;
    }

    return TRUE;
}


/*==============================================================================
Description: ��ѭ����Ϣ������ȡ��һ����Ϣ����������Ϣ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL GUIIM_NOTIFY_PACK_T * GetNotify(CTRLIM_OBJ_T *im_ptr)
{
    GUIIM_NOTIFY_PACK_T * pack_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return PNULL;
    }
    
    if (im_ptr->notify.pack_count == 0)
    {
        return PNULL;
    }

    pack_ptr = &im_ptr->notify.pack[im_ptr->notify.pack_index];

    im_ptr->notify.pack_count -= 1;
    im_ptr->notify.pack_index += 1;

    if (im_ptr->notify.pack_index >= GUIIM_NOTIFY_QUEUE_LEN)
    {
        im_ptr->notify.pack_index %= GUIIM_NOTIFY_QUEUE_LEN;
    }

    return pack_ptr;
}



/*==============================================================================
Description: DealNotify
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void DealNotify(CTRLIM_OBJ_T *im_ptr)
{
    GUIIM_NOTIFY_PACK_T * notify_ptr = PNULL;
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T *)im_ptr;

    while (PNULL != (notify_ptr = GetNotify(im_ptr)))
    {
        //�����ڴ���Ĺ����г������뷨�ؼ������ٺ���Ȼ������Ϣ�����
        if (PNULL == MMK_GetCtrlPtr(ctrl_ptr->handle)) break;

        CommitData(im_ptr, notify_ptr);
    }

    return;
}


/*==============================================================================
Description: ����һ�����ư���
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �ڵ�����������һ���ڵ㣬����Ѿ�����ͬ�ģ���ȥ��֮ǰ��
==============================================================================*/
LOCAL void AddCustomKey(CTRLIM_OBJ_T *im_ptr, GUIIM_CUSTOM_KEY_PARAM_T* key_info_ptr)
{
    GUIIM_CUSTOM_KEY_PARAM_T *new_info_ptr = PNULL;
    GUIIM_CUSTOM_KEY_PARAM_T *cur_info_ptr = PNULL;
    GUIIM_CUSTOM_KEY_PARAM_T *next_info_ptr = PNULL;

    //kevin.lou modified:delete assert    
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != key_info_ptr); /*assert verified*/
    if ((PNULL == im_ptr)||(PNULL == key_info_ptr))
    {
        return;
    }

    //�����½ڵ�
    new_info_ptr = (GUIIM_CUSTOM_KEY_PARAM_T*)SCI_ALLOC_APP(sizeof(GUIIM_CUSTOM_KEY_PARAM_T));
    SCI_MEMSET(new_info_ptr, 0, sizeof(GUIIM_CUSTOM_KEY_PARAM_T));

    *new_info_ptr = *key_info_ptr;
    new_info_ptr->next_ptr = im_ptr->custom_key_ptr;
    im_ptr->custom_key_ptr = new_info_ptr;

    //����abc��english�ȣ��˴�ֻ��Ҫabc
    if (GUIIM_TYPE_ABC == new_info_ptr->im_type)
    {
        new_info_ptr->im_type = GUIIM_TYPE_MULTITAP;
    }


    //ȥ��֮ǰ��ͬ�����͵Ľڵ㣬������ڵ�������ȫ������ȥ��֮ǰ���е�
    //��Ϊ���������ò���̫�࣬��������дû��Ч������
    cur_info_ptr = im_ptr->custom_key_ptr;
    next_info_ptr = cur_info_ptr->next_ptr;

    //���next�Ƿ��new��ͬ
    while (PNULL != next_info_ptr)
    {

        if (next_info_ptr->key == new_info_ptr->key
                && (next_info_ptr->im_type == new_info_ptr->im_type))
        {
            cur_info_ptr->next_ptr = next_info_ptr->next_ptr;
            SCI_FREE(next_info_ptr);

            next_info_ptr = cur_info_ptr->next_ptr;
        }
        else
        {
            cur_info_ptr = cur_info_ptr->next_ptr;
            next_info_ptr = cur_info_ptr->next_ptr;
        }
    }

    return;
}


/*==============================================================================
Description: ���ö�����Ϣ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetCustomKey(CTRLIM_OBJ_T *im_ptr, GUIIM_CUSTOM_KEY_PARAM_T *key_info_ptr)
{
    GUIIM_EVENT_DATA_U data = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != key_info_ptr); /*assert verified*/
    if ((PNULL == im_ptr)||(PNULL == key_info_ptr))
    {
        return FALSE;
    }

    if (PNULL == im_ptr->im_handle || PNULL == im_ptr->apis_ptr)
    {
        return FALSE;
    }

    data.custom_key_info_ptr = key_info_ptr;

    (void)(im_ptr->apis_ptr->handle)(im_ptr->im_handle, GUIIM_EVENT_CUSTOM_KEY, &data);

    return TRUE;
}


/*==============================================================================
Description: �������еĶ�����Ϣ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SetAllCustomKey(CTRLIM_OBJ_T *im_ptr)
{
    GUIIM_CUSTOM_KEY_PARAM_T *cur_info_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != im_ptr); /*assert verified*/
    if (PNULL == im_ptr)
    {
        return;
    }

    cur_info_ptr = im_ptr->custom_key_ptr;

    while (PNULL != cur_info_ptr)
    {
        if (GUIIM_TYPE_NONE == cur_info_ptr->im_type
                || im_ptr->method_param.im_def_ptr->type == cur_info_ptr->im_type)
        {
            SetCustomKey(im_ptr, cur_info_ptr);
        }

        cur_info_ptr = cur_info_ptr->next_ptr;
    }

    return;
}


/*==============================================================================
Description: ReleaseCustomKey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �ͷ��ڴ�
==============================================================================*/
LOCAL void ReleaseCustomKey(CTRLIM_OBJ_T *im_ptr)
{
    GUIIM_CUSTOM_KEY_PARAM_T * next_custom_key_ptr = PNULL;
    GUIIM_CUSTOM_KEY_PARAM_T * cur_custom_key_ptr = PNULL;

    if (PNULL == im_ptr)
    {
        return;
    }

    //��ն��ư����б�
    next_custom_key_ptr = im_ptr->custom_key_ptr;

    while (PNULL != next_custom_key_ptr)
    {
        cur_custom_key_ptr = next_custom_key_ptr;
        next_custom_key_ptr = cur_custom_key_ptr->next_ptr;
        SCI_FREE(cur_custom_key_ptr);
    }

    im_ptr->custom_key_ptr = PNULL;

    return;
}


/*==============================================================================
Description: CloseAppendWin
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 
==============================================================================*/
LOCAL void CloseAppendWin(CTRLIM_OBJ_T *im_ptr)
{
    GUIIM_APPEND_WIN_NODE_T *cur_node_ptr = PNULL;
    GUIIM_APPEND_WIN_NODE_T *release_node_ptr = PNULL;


    cur_node_ptr = im_ptr->append_win_ptr;
    im_ptr->append_win_ptr = PNULL;

    while (PNULL != cur_node_ptr)
    {
        release_node_ptr = cur_node_ptr;
        cur_node_ptr = cur_node_ptr->next;

        if (MMK_IsNeedCloseWin(release_node_ptr->win_handle))
        {
            MMK_CloseWin(release_node_ptr->win_handle);
        }
        
        SCI_FREE(release_node_ptr);
    }

    return;
}




/*==============================================================================
Description: �ж���msg�Ƿ������л�����������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �κ��������İ��������������л�
�����������򵥵Ķ���Ϊ����һ�������������������
==============================================================================*/
LOCAL BOOLEAN IsSwitchMsg(MMI_MESSAGE_ID_E msg_id)
{
    BOOLEAN is_switch_key = FALSE;

    is_switch_key = (MSG_APP_0 == msg_id
        || MSG_APP_1 == msg_id
        || MSG_APP_2 == msg_id
        || MSG_APP_3 == msg_id
        || MSG_APP_4 == msg_id
        || MSG_APP_5 == msg_id
        || MSG_APP_6 == msg_id
        || MSG_APP_7 == msg_id
        || MSG_APP_8 == msg_id
        || MSG_APP_9 == msg_id
        || MSG_APP_HASH == msg_id
        || MSG_APP_STAR == msg_id
#if defined(KEYPAD_TYPE_QWERTY_KEYPAD)
        || MSG_APP_Q == msg_id
        || MSG_APP_W == msg_id
        || MSG_APP_E == msg_id
        || MSG_APP_R == msg_id
        || MSG_APP_T == msg_id
        || MSG_APP_Y == msg_id
        || MSG_APP_U == msg_id
        || MSG_APP_I == msg_id
        || MSG_APP_O == msg_id
        || MSG_APP_P == msg_id
        || MSG_APP_A == msg_id
        || MSG_APP_S == msg_id
        || MSG_APP_D == msg_id
        || MSG_APP_F == msg_id
        || MSG_APP_G == msg_id
        || MSG_APP_H == msg_id
        || MSG_APP_J == msg_id
        || MSG_APP_K == msg_id
        || MSG_APP_L == msg_id
        || MSG_APP_DEL == msg_id
        || MSG_APP_Z == msg_id
        || MSG_APP_X == msg_id
        || MSG_APP_C == msg_id
        || MSG_APP_V == msg_id
        || MSG_APP_B == msg_id
        || MSG_APP_N == msg_id
        || MSG_APP_M == msg_id
        || MSG_APP_COMMA == msg_id
        || MSG_APP_PERIOD == msg_id
        || MSG_APP_ENTER == msg_id
        || MSG_APP_FN == msg_id
        || MSG_APP_SHIFT == msg_id
        || MSG_APP_AT_QWERTY == msg_id
        || MSG_APP_SPACE == msg_id
        || MSG_APP_AND == msg_id
        || MSG_APP_QUESTION == msg_id
        || MSG_APP_CTRL == msg_id
        || MSG_APP_PLUS == msg_id
        || MSG_APP_LPARENTHESIS == msg_id
        || MSG_APP_RPARENTHESIS == msg_id
        || MSG_APP_MINUS == msg_id
        || MSG_APP_DQUOTES == msg_id
        || MSG_APP_SEMICOLON == msg_id
        || MSG_APP_COLON == msg_id
        || MSG_APP_SLASH == msg_id
        || MSG_APP_SAND == msg_id
        || MSG_APP_EXCLAMATION == msg_id
#endif
        );

    return is_switch_key;
}



//for app

/*==============================================================================
Description: ����������Ϣѭ����������һ����Ϣ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �ú���������Ӧ�õ��õģ��ڴ���ϵͳ��Ϣ��ʱ��
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_SetNotify(
    MMI_HANDLE_T im_handle,
    GUIIM_NOTIFY_PACK_T *pack_data_ptr
)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);
    int8 cur_index = 0;

    if (PNULL == im_ptr || PNULL == pack_data_ptr)
    {
        return FALSE;
    }

    if (im_ptr->notify.pack_count >= GUIIM_NOTIFY_QUEUE_LEN)
    {
        return FALSE;
    }

    cur_index = im_ptr->notify.pack_index + im_ptr->notify.pack_count;

    if (cur_index >= GUIIM_NOTIFY_QUEUE_LEN)
    {
        cur_index %= GUIIM_NOTIFY_QUEUE_LEN;
    }

    im_ptr->notify.pack[cur_index] = *pack_data_ptr;
    im_ptr->notify.pack_count += 1;

    return TRUE;
}



/*==============================================================================
Description: �ڴ����ϵͳ��Ϣ��ʱ���ύ������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �ú���������Ӧ�õ��õ�
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_CommitData(
    MMI_HANDLE_T im_handle,
    GUIIM_NOTIFY_PACK_T *pack_data_ptr
)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    if (PNULL == pack_data_ptr)
    {
        return FALSE;
    }

    return CommitData(im_ptr, pack_data_ptr);
}

#if 0
/*==============================================================================
Description: ��ȡ���뷨����ʾͼ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_IMAGE_ID_T GUIIM_GetImIcon(MMI_HANDLE_T im_handle)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return 0;
    }

    if (PNULL == im_ptr->method_param.im_def_ptr)
    {
        return 0;
    }

    return MMITHEME_ImGetIcon(im_ptr->method_param.im_def_ptr->method);
}
#endif

/*==============================================================================
Description: ��ȡ���뷨����ʾ����
Global resource dependence: NONE
Author: haiwu.chen
Note: 
==============================================================================*/
PUBLIC MMI_STRING_T CTRLIM_ImGetText(MMI_HANDLE_T im_handle)
{
    MMI_STRING_T sz_Text = {0};
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return sz_Text;
    }

    if (PNULL == im_ptr->method_param.im_def_ptr)
    {
        return sz_Text;
    }

    return MMITHEME_ImGetText(im_ptr->method_param.im_def_ptr->method);
}


/*==============================================================================
Description: ��ȡ���뷨����ʾ���򣬼�������ʾui�Ĳ���
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ע��ʹ�����Ϣ��ȡ���������
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_GetImRect(
    MMI_HANDLE_T im_handle,
    GUI_RECT_T   *rect_ptr
)
{
    BOOLEAN  result = FALSE;
    GUIIM_EVENT_DATA_U data = {0};
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    //wrong parameter
    if (PNULL == rect_ptr)
    {
        return FALSE;
    }

    //not init...
    if (PNULL == im_ptr->apis_ptr)
    {
        return FALSE;
    }

    result = (im_ptr->apis_ptr->handle)(im_ptr->im_handle, GUIIM_EVENT_GET_RECT, &data);

    if (result)
    {
        *rect_ptr = data.get_rect;
    }

    return result;
}


/*==============================================================================
Description: GUIIM_GetInput
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC GUIIM_INPUT_TYPE_T CTRLIM_GetInput(MMI_HANDLE_T im_handle)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return GUIIM_INPUT_TYPE_NONE;
    }

    return im_ptr->method_param.im_def_ptr->input;
}


/*==============================================================================
Description: GUIIM_GetType
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC GUIIM_TYPE_T CTRLIM_GetType(MMI_HANDLE_T im_handle)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return GUIIM_TYPE_NONE;
    }

    return im_ptr->method_param.im_def_ptr->type;
}


/*==============================================================================
Description: GUIIM_GetCapital
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC GUIIM_CAPITAL_MODE_T CTRLIM_GetCapital(MMI_HANDLE_T im_handle)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return GUIIM_MODE_MAX;
    }

    return im_ptr->method_param.im_def_ptr->capital;
}

/*==============================================================================
Description: GUIIM_SetCapital
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_SetCapital(
								MMI_HANDLE_T			im_handle,
								GUIIM_CAPITAL_MODE_T	cap
								)
{
	BOOLEAN			result = FALSE;
    CTRLIM_OBJ_T	*im_ptr = GetImPtr(im_handle);

	if (PNULL != im_ptr)
	{
		result = SetCap(im_ptr,cap);
	}

	return (result);
}

/*==============================================================================
Description: GUIIM_SetInput
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_SetInput(MMI_HANDLE_T im_handle, GUIIM_INPUT_TYPE_T input)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    return SetInput(im_ptr, input);
}


/*==============================================================================
Description: GUIIM_SetType
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_SetType(MMI_HANDLE_T im_handle, GUIIM_TYPE_T type)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    return SetType(im_ptr, type);
}


//���뷨�����趨����
/*==============================================================================
Description: �����֮ǰ�����ã���������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_SetInputSet(MMI_HANDLE_T im_handle, GUIIM_INPUT_TYPE_T input_set)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    SetInputSet(im_ptr, input_set);


    return TRUE;
}


/*==============================================================================
Description: ���������ر�ָ��������Ҫ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_SetTypeSet(MMI_HANDLE_T im_handle, GUIIM_TYPE_T type_set)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    SetTypeSet(im_ptr, type_set);


    return TRUE;
}


/*==============================================================================
Description: ��յ�ǰ���뷨�����뻺�棬���������У������ʼ��״̬
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void CTRLIM_ClearState(MMI_HANDLE_T im_handle)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return;
    }

    (void)(im_ptr->apis_ptr->handle)(im_ptr->im_handle, GUIIM_EVENT_SLEEP, PNULL);

    DealNotify(im_ptr);

    SCI_MEMSET(&(im_ptr->last_commit), 0, sizeof(im_ptr->last_commit));

    return;
}



/*==============================================================================
Description: GUIIM_DisplaySoftkey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 
����ֵΪ - ���뷨�ؼ��Ƿ���ʾ��softkey
ע���� DisplaySoftkey() ������ - ��������ķ���ֵΪ�Ƿ���Ҫ֪ͨ���ؼ�ˢ��
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_DisplaySoftkey(
    MMI_HANDLE_T im_handle
)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    (void)DisplaySoftkey(im_ptr);


    return (GUIIM_EDIT_NONE != im_ptr->edit_state_bak);
}


/*==============================================================================
Description: �����뷨��Ϊ��active�ؼ�����ʱ�������Ϊ�������к��ַ�����ת����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLIM_HandleMsg(
    MMI_HANDLE_T im_handle,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    return ImHandleMsg((CTRLBASE_OBJ_T *)im_ptr, msg_id, param);
}



/*==============================================================================
Description: GUIIM_SetCustomKey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �ض���Ӧ�ã�������������İ�����
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_SetCustomKey(
    MMI_HANDLE_T im_handle,
    GUIIM_TYPE_T im_type,
    GUIIM_CUSTOM_KEY_T key,
    wchar* key_values,
    uint32 key_count
)
{
    BOOLEAN result = FALSE;
    GUIIM_CUSTOM_KEY_PARAM_T key_info = {0};
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    key_info.im_type = im_type;
    key_info.key = key;
    key_info.key_count = key_count;

    if (PNULL != key_values && 0 != key_count)
    {
        key_count = MIN(key_count, GUIIM_CUSTOM_CHAR_LIMIT);
        SCI_MEMCPY(key_info.key_values, key_values, (key_count*sizeof(key_values[0])));
    }

    AddCustomKey(im_ptr, &key_info);

    result = SetCustomKey(im_ptr, &key_info);

    return result;
}


/*****************************************************************************/
//  Description : is only input ascii
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLIM_IsOnlyInputAscii(MMI_HANDLE_T im_handle)
{
    BOOLEAN     result = FALSE;
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    if ((im_ptr->method_param.type_set & GUIIM_TYPE_SMART) ||
        (im_ptr->method_param.type_set & GUIIM_TYPE_STROKE) ||
        (im_ptr->method_param.type_set & GUIIM_TYPE_MULTITAP))
    {
        result = FALSE;
    }
    else
    {
        result = TRUE;
    }

    return (result);
}

/*==============================================================================
Description: GUIIM_CommitLast
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void CTRLIM_CommitLast(MMI_HANDLE_T im_handle)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);
    BOOLEAN has_commit = FALSE;

    if (PNULL == im_ptr)
    {
        return;
    }

    //0 deal the notify queue, may be the last commit has been committed...  
    DealNotify(im_ptr);
    
    // 1 commit if there is ...
    has_commit = CommitLast(im_ptr);

    if (has_commit)
    {
        // 2 set app to init
        (void)(im_ptr->apis_ptr->handle)(im_ptr->im_handle, GUIIM_EVENT_FORCE_COMMIT, PNULL);

        DealNotify(im_ptr);
    }


    return;
}


/*==============================================================================
Description: GUIIM_AppendWin
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 
==============================================================================*/
PUBLIC void CTRLIM_AppendWin(
    MMI_HANDLE_T im_handle,
    CAF_HANDLE_T win_handle
)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);
    GUIIM_APPEND_WIN_NODE_T *new_node_ptr = PNULL;
    GUIIM_APPEND_WIN_NODE_T *cur_node_ptr = PNULL;
    GUIIM_APPEND_WIN_NODE_T *head_node_ptr = PNULL;


    if (PNULL == im_ptr)
    {
        return;
    }

    cur_node_ptr = im_ptr->append_win_ptr;

    while (PNULL != cur_node_ptr)
    {
        if (win_handle == cur_node_ptr->win_handle) return;
        cur_node_ptr = cur_node_ptr->next;
    }

    new_node_ptr = SCI_ALLOC_APP(sizeof(GUIIM_APPEND_WIN_NODE_T));
    SCI_MEMSET(new_node_ptr, 0, sizeof(GUIIM_APPEND_WIN_NODE_T));
    new_node_ptr->win_handle = win_handle;

    head_node_ptr = im_ptr->append_win_ptr;
    im_ptr->append_win_ptr = new_node_ptr;
    
    im_ptr->append_win_ptr->next = head_node_ptr;

    return;
}


/*==============================================================================
Description: GUIIM_RemoveWin
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 
==============================================================================*/
PUBLIC void CTRLIM_RemoveWin(
    MMI_HANDLE_T im_handle,
    CAF_HANDLE_T win_handle
)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);
    GUIIM_APPEND_WIN_NODE_T *cur_node_ptr = PNULL;
    GUIIM_APPEND_WIN_NODE_T *prev_node_ptr = PNULL;

    if (PNULL == im_ptr)
    {
        return;
    }

    cur_node_ptr = im_ptr->append_win_ptr;

    while (PNULL != cur_node_ptr)
    {
        if (win_handle == cur_node_ptr->win_handle)
        {
            if (PNULL == prev_node_ptr)
            {
                im_ptr->append_win_ptr = cur_node_ptr->next;
            }
            else
            {
                prev_node_ptr->next = cur_node_ptr->next;
            }

            SCI_FREE(cur_node_ptr);

            break;
        }
        
        prev_node_ptr = cur_node_ptr;
        cur_node_ptr = cur_node_ptr->next;
    }

    return;
}


/*==============================================================================
Description: GUIIM_EnableSymbol
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_SetInputSymbol(MMI_HANDLE_T im_handle, BOOLEAN is_banned)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    if (is_banned != im_ptr->init_param.is_ban_symbol)
    {
        im_ptr->init_param.is_ban_symbol = is_banned;
        ResetLimit(im_ptr);
    }

    return TRUE;
}


/*==============================================================================
Description: �������뷨�ĸ�������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ������tag֮�󣬼򵥵��ؽ����뷨Ӧ��
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_SetImTag(MMI_HANDLE_T im_handle, GUIIM_TAG_T tag)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    if (tag != im_ptr->method_param.tag)
    {
        im_ptr->method_param.tag = tag;
        //term first, if need
        AppTerm(im_ptr);
        //init
        AppInit(im_ptr);
    }

    return TRUE;
}


/*==============================================================================
Description: ����Ƿ���Դӵ�ǰ�����뷽ʽ�л�����������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CTRLIM_CanSwithToTp(
    MMI_HANDLE_T im_handle,
    GUIIM_INPUT_TYPE_T *default_input
)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    return CanSwitchToTp(im_ptr, default_input);
}


/*==============================================================================
Description: Get method paramet
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC GUIIM_METHOD_PARAM_T * CTRLIM_GetImMethodParam(
    MMI_HANDLE_T im_handle
)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return FALSE;
    }

    return &(im_ptr->method_param);
}


/*==============================================================================
Description: Get im handle
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void* CTRLIM_GetImHandle(
    MMI_HANDLE_T im_handle
)
{
    CTRLIM_OBJ_T *im_ptr = GetImPtr(im_handle);

    if (PNULL == im_ptr)
    {
        return PNULL;
    }

    return im_ptr->im_handle;
}

