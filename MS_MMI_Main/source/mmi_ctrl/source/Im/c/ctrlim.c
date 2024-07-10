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
20101018 增加实例输入语言的设置
1 当输入无限制且无指定输入语言的时候，才获取默认的输入
2 当有指定语言输入的时候，切换过程中不可切换语言
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
Note: 理论上本函数应该可以返回FALSE，但是一旦false，系统就崩溃了
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
Note: 空函数!!! 此处不能调用系统的GUICTRL_SetRect, 否则rect会被清零
==============================================================================*/
LOCAL BOOLEAN ImSetRect(
    CTRLBASE_OBJ_T          *ctrl_ptr,
    const CAF_RECT_T    *rect_ptr
);

/*==============================================================================
Description: HandleGetFocus
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 返回是否重建了输入法
!!! 改返回值不是消息是否处理
==============================================================================*/
LOCAL BOOLEAN HandleGetFocus(CTRLIM_OBJ_T *im_ptr);

LOCAL void ResetLimit(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: 将设置的输入类型设置转化为普通限制
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL GUIIM_LIMIT_T ConvertTypeToLimit(GUIIM_TYPE_T type_set);

/*==============================================================================
Description: 将english、abc转换成对应的智能和多按键输入
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 符号也转成智能，按说是不会出现符号的
==============================================================================*/
LOCAL GUIIM_TYPE_T ConvertType(GUIIM_TYPE_T type);

/*==============================================================================
Description: 从初始化参数获取实例输入语言
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL GUIIM_LANGUAGE_T GetInputLang(GUIIM_INIT_PARAM_T *init_param_ptr);

/*==============================================================================
Description: 检测是否可以从当前的输入方式切换至键盘输入
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN CanSwitchToKeyboard(
    CTRLIM_OBJ_T *im_ptr,
    GUIIM_INPUT_TYPE_T *default_input
);

/*==============================================================================
Description: 检测是否可以从当前的输入方式切换至键盘输入
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN CanSwitchToTp(
    CTRLIM_OBJ_T *im_ptr,
    GUIIM_INPUT_TYPE_T *default_input
);

/*==============================================================================
Description: 初始化输入法
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN AppInit(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: 销毁输入法实例
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void AppTerm(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: 输入法控件首先将消息交由输入法应用处理，再视处理结果决定下一步处理
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 返回应用是否处理了改系统消息
==============================================================================*/
LOCAL BOOLEAN AppHandle(CTRLIM_OBJ_T *im_ptr, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*==============================================================================
Description: 强制应用刷新显示
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void AppDraw(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: 提交未提交的字符
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 返回值表示是否有提交
==============================================================================*/
LOCAL BOOLEAN CommitLast(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: 应用更新method之后，控件同步属性值
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN ResetParameter(CTRLIM_OBJ_T *im_ptr, GUIIM_METHOD_T method);

/*==============================================================================
Description: 当输入法参数设置之后，输入法有可能会发生改变。
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
比如语言的变更就不影响手写和数字输入
大小写的变更也不影响不区分大小写的输入
==============================================================================*/
LOCAL BOOLEAN SetParameter(CTRLIM_OBJ_T *im_ptr, GUIIM_METHOD_CFG_T *im_def_ptr);

//---------------
//set 系列函数
//---------------
//set parameter
//这个系列的函数都是获取新的输入法，并设置
//通常，设置错误发生在设置输入法中，都是错误，必须避免这种情况的发生
/*==============================================================================
Description: 重新设置输入法之后的设置
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
Description: 设置允许的输入方式集合
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetInputSet(CTRLIM_OBJ_T *im_ptr, GUIIM_INPUT_TYPE_T input_set);

/*==============================================================================
Description: 设置允许的输入类型集合
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetTypeSet(CTRLIM_OBJ_T *im_ptr, GUIIM_TYPE_T type_set);

/*==============================================================================
Description: 触笔响应区域
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SetCtrlRect(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: 检查输入法编辑状态的变化，并且在有变化的时候刷新
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void CheckEditState(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: DisplaySoftkey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 返回当前的编辑状态，而非显示与否，因为编辑状态也可能无需重新显示
==============================================================================*/
LOCAL BOOLEAN DisplaySoftkey(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: 通过输入法空间的实例数据，向输入法控件的父控件提交字符串
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 向创建输入法的控件提交输入法应用的提交消息
==============================================================================*/
LOCAL BOOLEAN CommitData(
    CTRLIM_OBJ_T *im_ptr,
    GUIIM_NOTIFY_PACK_T *pack_data_ptr
);

/*==============================================================================
Description: 从循环消息队列中取出一个消息，并更新消息数
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
Description: 增加一个定制按键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 在单链表中增加一个节点，如果已经有相同的，则去掉之前的
==============================================================================*/
LOCAL void AddCustomKey(CTRLIM_OBJ_T *im_ptr, GUIIM_CUSTOM_KEY_PARAM_T* key_info_ptr);

/*==============================================================================
Description: 设置定制信息
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN SetCustomKey(CTRLIM_OBJ_T *im_ptr, GUIIM_CUSTOM_KEY_PARAM_T *key_info_ptr);

/*==============================================================================
Description: 设置所有的定制信息
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SetAllCustomKey(CTRLIM_OBJ_T *im_ptr);

/*==============================================================================
Description: ReleaseCustomKey
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 释放内存
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

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->SetRect = ImSetRect;
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
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
Note: 理论上本函数应该可以返回FALSE，但是一旦false，系统就崩溃了
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

    //用系统支持的输入类型和输入方式集合来修正当前的设置
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

    //设置默认输入法
    //1 没有限制
    //2 没有指定输入语言
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

    //未获取输入法，则根据限制和设定获取输入法
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

    //释放定制按键的信息
    ReleaseCustomKey(im_ptr);

    //清空消息队列, 主要是避免输入法控件销毁之后还有消息需要处理
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

    //lost focus, 统一处理提交
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

    // 此处引入2倍TRUE的概念，其实是返回值不够用了，为了不引起较大的修改，暂如此
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
            
            //有可能从更多窗口设置了新的输入语言，导致输入法发生改变
            if (input_lang != im_ptr->method_param.im_def_ptr->lang
                && GUIIM_LANG_NONE != input_lang
                && GUIIM_LANG_NONE != im_ptr->method_param.im_def_ptr->lang)
            {
                //输入语言发生改变
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
                //如果应用未处理，则强制销毁实例，重建
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

        //透传给上层处理，先清空缓存的提交信息
        SCI_MEMSET(&(im_ptr->last_commit), 0, sizeof(im_ptr->last_commit));
        result = MMI_RESULT_FALSE;
        break;

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:

        if (result_app) break;

        //此处首先处理焦点切换的问题。
        //在编辑控件之外的部分点击可获得焦点的其他控件，会引起焦点切换
        if (im_ptr->method_param.im_def_ptr->method == GUIIM_M_HANDWRITING)     //手写时，需要处理此处的消息
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

            // 1 当点在编辑控件外
            // 2 当点下的控件被激活
            //则跳出，否则
            // 1   激活触笔输入法
            // 2 不处理

            if (!GUI_PointIsInRect(tp_point, parent_rect))
            {
                MMK_GetHandleByPos(tp_point, TRUE, &win_handle, &ctrl_handle, ctrl_ptr->handle);

                if ((0 != ctrl_handle) &&
                    (!MMK_IsAncestorCtrl(ctrl_handle,ctrl_ptr->handle)))
                {
                    is_actived = MMK_SetActiveCtrl(ctrl_handle, FALSE);

                    if (!is_actived)
                    {
                        //在send msg期间，加上消息资源锁
                        im_ptr->is_msg_band = TRUE;
                        MMK_SendMsg(ctrl_handle, MSG_TP_PRESS_DOWN, param);
                        MMK_SendMsg(ctrl_handle, MSG_TP_PRESS_UP, param);
                        im_ptr->is_msg_band = FALSE;
                    }

                    break;
                }
            }

            //如果非触笔输入，则切换至触笔输入
            //是点击还是手写，由theme决定
            if (CanSwitchToTp(im_ptr, &default_input))
            {
                SetInput(im_ptr, default_input);
            }
            else        //触笔输入，有可能需要切换光标
            {
                MMK_GetHandleByPos(tp_point, TRUE, &win_handle, &ctrl_handle, ctrl_ptr->handle);
                
                //在send msg期间，加上消息资源锁
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

        //判定是否需要切换至键盘输入
        if (IsSwitchMsg(msg_id) && CanSwitchToKeyboard(im_ptr, &default_input))
        {
            BOOLEAN is_set = FALSE;
            
            /*NEWMS00180411 主要是如果NV里面是手写输入法，然后从tp->keyboard
            找到一个最相似的输入法，会是123输入法，此时，再调出TP，
            则会跑到TP的123输入法，导致无法切换问题，修改方案，直接切换到
            智能输入法，前提是手写依赖于智能
            */
            if (GUIIM_M_HANDWRITING == im_ptr->method_param.im_set.method)
            {
                im_ptr->method_param.im_set.type = GUIIM_TYPE_SMART;
            }

            is_set = SetInput(im_ptr, default_input);

            //使能该键，作为一次有效输入
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
Note: 空函数!!! 此处不能调用系统的GUICTRL_SetRect, 否则rect会被清零
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
Note: 返回是否重建了输入法
!!! 改返回值不是消息是否处理
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
        // 是否有显示大小的变化
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
Description: 设置limit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: limit由多个条件计算得到。
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
Description: 将设置的输入类型设置转化为普通限制
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
    1 smart是特指的本国语言只能输入,如果不支持smart，则禁止local
      认为仅有支持某国smart的之后才支持改过其他的特有输入，比如拼音和笔划总是同
      时支持的
    2 如果不支持english & abc，则禁止english
      认为英文总是同时支持smart和abc的
==============================================================================*/
LOCAL GUIIM_LIMIT_T ConvertTypeToLimit(GUIIM_TYPE_T type_set)
{
    GUIIM_LIMIT_T limit = 0;

    //PDA的输入法切换逻辑与之前的不同，在我没有想出一个周全的办法之前，先如此处理
    //zhoulianxiang 20110914
#if 0
#if defined(MMI_PDA_SUPPORT)
    if (!(GUIIM_TYPE_SMART & type_set))
#else
    if (!(GUIIM_TYPE_SMART & type_set) && !(GUIIM_TYPE_ENGLISH & type_set))
#endif
#else
    //NEWMS00186633 [6530][64x64_240x320TK_OP][建议类]编辑互联网账户用户名时，
    //取消虚拟键盘中输入法切换按钮allow_im = GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL;
    //导致可以输入中文
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
Description: 将english、abc转换成对应的输入
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
#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU)    //可能不存在智能输入法
    else if (type == GUIIM_TYPE_ENGLISH)
    {
        new_type = GUIIM_TYPE_SMART;
    }
#endif
    
    return new_type;
}


/*==============================================================================
Description: 从初始化参数获取实例输入语言
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 需要处理目前的特殊情况 - english\abc\123 同时支持
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
Description: 检测是否可以从当前的输入方式切换至键盘输入
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

    //当前键盘输入
    if ((GUIIM_INPUT_TYPE_KEYBOARD == im_ptr->method_param.im_set.input)
            || (GUIIM_INPUT_TYPE_QWERT == im_ptr->method_param.im_set.input))
    {
        return FALSE;
    }

    *default_input = MMITHEME_IMGetKbDefault(im_ptr->method_param.input_set);

    //无键盘输入
    if (GUIIM_INPUT_TYPE_NONE == *default_input)
    {
        return FALSE;
    }

    return TRUE;
}


/*==============================================================================
Description: 检测是否可以从当前的输入方式切换至键盘输入
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

    //当前触笔输入
    if ((GUIIM_INPUT_TYPE_TOUCHPEN == im_ptr->method_param.im_set.input)
            || (GUIIM_INPUT_TYPE_TP_QWERT == im_ptr->method_param.im_set.input)
            || (GUIIM_INPUT_TYPE_HANDWRITING == im_ptr->method_param.im_set.input))
    {
        return FALSE;
    }

    *default_input = MMITHEME_IMGetTpDefault(im_ptr->method_param.input_set);

    //无键盘输入
    if (GUIIM_INPUT_TYPE_NONE == *default_input)
    {
        return FALSE;
    }

    return TRUE;
}



/*==============================================================================
Description: 初始化输入法
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

    //参数检查，防止内存泄漏
    SCI_ASSERT(PNULL == im_ptr->im_handle); /*assert verified*/
    //设置的参数检查，所以的设置以 method_param.im_def_ptr 为准
    SCI_ASSERT(PNULL != im_ptr->method_param.im_def_ptr); /*assert verified*/

    //检测输入法的合法性
    //如果非法，则根据是否有替代输入法进入下一步
    //如果没有替代输入法，则assert
    is_valid_method = MMITHEME_IMCheckMethod(im_ptr->method_param.im_def_ptr,
                                             &new_method);

    if (!is_valid_method)
    {
        SCI_ASSERT(GUIIM_M_NONE != new_method); /*assert verified*/
        ResetParameter(im_ptr, new_method);
    }


    //总是重新获取输入法api
    im_ptr->apis_ptr = MMITHEME_IMGetApis(im_ptr->method_param.im_def_ptr->method);
    //如果没有相应的api定义，那么一定是系统级的错误了
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

    //初始化的后续处理
    SetAllCustomKey(im_ptr);
    SetCtrlRect(im_ptr);

    //AppDraw, 需要刷新一次，不能依赖系统刷新，否则会有刷新延迟
    if ( MMK_IsFocusWin( im_ptr->win_handle ))
    {
        AppDraw(im_ptr);
    }

    SCI_TRACE_LOW ("GUIIM AppInit im_ptr == %x method == %x", im_ptr, init_data.method_param_ptr->im_def_ptr->method);
    return TRUE;
}


/*==============================================================================
Description: 销毁输入法实例
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
Description: 输入法控件首先将消息交由输入法应用处理，再视处理结果决定下一步处理
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 返回应用是否处理了改系统消息
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
Description: 强制应用刷新显示
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
    //因当前程序存在一种情况，即窗口创建时，setactive_edit而启动了输入法，但这个窗口却不是focus状态
    if ( !MMK_IsFocusWin( im_ptr->win_handle ))
    {
        return;
    }
#endif

    (void)(im_ptr->apis_ptr->handle)(im_ptr->im_handle, GUIIM_EVENT_PAINT, PNULL);

    return;
}


/*==============================================================================
Description: 提交未提交的字符
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 返回值表示是否有提交
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

    //提交未提交的字符
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
Description: 应用更新method之后，控件同步属性值
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

    //不同的输入法是不能有同样的配置的
    SCI_ASSERT(im_def_ptr != im_ptr->method_param.im_def_ptr); /*assert verified*/

    im_ptr->method_param.im_def_ptr = im_def_ptr;

    //将当前设置与标准属性定义同步
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
Description: 当输入法参数设置之后，输入法有可能会发生改变。
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
比如语言的变更就不影响手写和数字输入
大小写的变更也不影响不区分大小写的输入
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
    
    //销毁之前的输入法实例
    AppTerm(im_ptr);

    im_ptr->method_param.im_def_ptr = im_def_ptr;
    im_ptr->method_param.im_set.method = im_def_ptr->method;

    //重建输入法实例
    result_init = AppInit(im_ptr);

    if (result_init)
    {
        GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_SWICTH, PNULL, 0);
    }

    //通常，设置错误发生在设置输入法中，都是错误，必须避免这种情况的发生
    SCI_ASSERT(result_init); /*assert verified*/

    return result_init;
}



//---------------
//set 系列函数
//---------------
//set parameter
//这个系列的函数都是获取新的输入法，并设置
//通常，设置错误发生在设置输入法中，都是错误，必须避免这种情况的发生

/*==============================================================================
Description: 重新设置输入法之后的设置
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

    //销毁之前的输入法实例
    AppTerm(im_ptr);

    ResetParameter(im_ptr, method);
    
    //重建输入法实例
    result_init = AppInit(im_ptr);

    if (result_init)
    {
        GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_IM_SWICTH, PNULL, 0);
    }

    //通常，设置错误发生在设置输入法中，都是错误，必须避免这种情况的发生
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

    //語言校驗
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
    
    //手写输入会需要根据语言来初始化参数，而手写应用是无语言的。
    //这个 - 无语言 - 的设计，是输入法参数设置中的特例
#if 0 
    if (im_def_ptr == im_ptr->method_param.im_def_ptr)
    {
        return TRUE;
    }
#endif

    result_reset = SetParameter(im_ptr, im_def_ptr);

    //下面这些参数也需要再次更新保存，防止出现不一致的情况发生
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
Description: 设置允许的输入方式集合
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

    //新设定的方式不包含当前的方式，则获取当前限制下的默认方式
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
Description: 设置允许的输入类型集合
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

    //新设定的方式不包含当前的方式，则获取当前限制下的默认方式
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
Description: 触笔响应区域
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
    
    //未初始化，则不处理
    if (PNULL == im_ptr->im_handle || PNULL == im_ptr->apis_ptr)
    {
        return;
    }
    

    SCI_MEMSET(&(ctrl_ptr->rect), 0, sizeof(ctrl_ptr->rect));

//NEWMS00196109 [6530][64X64_320X240TK_QW]候选栏字符不支持 触笔点击上屏
//不支持触屏输入，则输入法控件区域为0；导致点击候选字区域时，消息无法到达输入法控件而无法处理
#if 0
    //无触笔输入，则无区域
    //一些特殊的应用通过设置无触笔来禁止输入法获取触笔消息
    if (!(GUIIM_INPUT_TYPE_TOUCHPEN & im_ptr->method_param.input_set)
            && !(GUIIM_INPUT_TYPE_TP_QWERT & im_ptr->method_param.input_set)
            && !(GUIIM_INPUT_TYPE_HANDWRITING & im_ptr->method_param.input_set))
    {
        return;
    }
#endif

    //直接从输入法应用获取控制区域
    is_got = (im_ptr->apis_ptr->handle)(im_ptr->im_handle,
                                        GUIIM_EVENT_GET_CTRL_RECT, &data);

    if (is_got) ctrl_ptr->rect = data.get_ctrl_rect;


    return;
}



/*==============================================================================
Description: 检查输入法编辑状态的变化，并且在有变化的时候刷新
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

    //如果是编辑状态转到非编辑状态，则通知上层控件刷新
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
Note: 返回是否控件已经处理了softkey，即是否需要通知父控件刷新
只有当从编辑状态变化到非编辑状态的时候，才需要编辑控件刷新softkey
1 字符输入状态
确定 x 删除
2 字符候选且有小标签状态
确定 x 返回
3 字符候选且无小标签状态
确定 x 取消
4 手写状态
保持不变
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
    
    //参数错误，啥也不做
    if (PNULL == im_ptr->im_handle || PNULL == im_ptr->apis_ptr)
    {
        return FALSE;
    }

    handle_result = (im_ptr->apis_ptr->handle)(im_ptr->im_handle, GUIIM_EVENT_GET_EDIT_STATE, &data);

    //获取失败，当作非编辑状态
    if (!handle_result)
    {
        data.edit_state = GUIIM_EDIT_NONE;
    }

    is_not_editing = (GUIIM_EDIT_NONE == data.edit_state);
    is_state_change = (data.edit_state != im_ptr->edit_state_bak);

    //编辑状态未变化，则不刷新
    if (!is_state_change)
    {
        return TRUE;
    }

    im_ptr->edit_state_bak = data.edit_state;

    //非编辑状态
    //状态变化，则要求父控件刷新softkey - 至此，肯定是状态变化的了
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
Description: 通过输入法空间的实例数据，向输入法控件的父控件提交字符串
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 向创建输入法的控件提交输入法应用的提交消息
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

        //处理提交信息
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

        //切换到全屏手写时，显示区域大小不一致，post刷新较慢，导致看到后面的窗口
        //因同步消息可能存在风险，故用宏控制起来
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

        //切换到全屏手写时，显示区域大小不一致
        //这个事情不能交给输入法应用自己处理，因为输入法应用是不知道自己的区域与其他的应用区域不一致
        //如果输入法应用不管其他的应用，每次调用一次MSG_NOTIFY_IM_CHANGE_RECT，则会导致在首次进入输入法时，
        //输入法应用里面存在一条未处理的消息即MSG_NOTIFY_IM_CHANGE_RECT
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
                /*NEWMS00180411 主要是如果NV里面是手写输入法，然后从tp->keyboard
                找到一个最相似的输入法，会是123输入法，此时，再调出TP，
                则会跑到TP的123输入法，导致无法切换问题，修改方案，直接切换到
                智能输入法，前提是手写依赖于智能
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
Description: 从循环消息队列中取出一个消息，并更新消息数
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
        //避免在处理的过程中出现输入法控件被销毁后仍然处理消息的情况
        if (PNULL == MMK_GetCtrlPtr(ctrl_ptr->handle)) break;

        CommitData(im_ptr, notify_ptr);
    }

    return;
}


/*==============================================================================
Description: 增加一个定制按键
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 在单链表中增加一个节点，如果已经有相同的，则去掉之前的
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

    //增加新节点
    new_info_ptr = (GUIIM_CUSTOM_KEY_PARAM_T*)SCI_ALLOC_APP(sizeof(GUIIM_CUSTOM_KEY_PARAM_T));
    SCI_MEMSET(new_info_ptr, 0, sizeof(GUIIM_CUSTOM_KEY_PARAM_T));

    *new_info_ptr = *key_info_ptr;
    new_info_ptr->next_ptr = im_ptr->custom_key_ptr;
    im_ptr->custom_key_ptr = new_info_ptr;

    //纠正abc，english等，此处只需要abc
    if (GUIIM_TYPE_ABC == new_info_ptr->im_type)
    {
        new_info_ptr->im_type = GUIIM_TYPE_MULTITAP;
    }


    //去除之前相同的类型的节点，如果本节点是设置全部，则去除之前所有的
    //因为这样的设置不会太多，所以这样写没有效率问题
    cur_info_ptr = im_ptr->custom_key_ptr;
    next_info_ptr = cur_info_ptr->next_ptr;

    //检查next是否和new相同
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
Description: 设置定制信息
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
Description: 设置所有的定制信息
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
Note: 释放内存
==============================================================================*/
LOCAL void ReleaseCustomKey(CTRLIM_OBJ_T *im_ptr)
{
    GUIIM_CUSTOM_KEY_PARAM_T * next_custom_key_ptr = PNULL;
    GUIIM_CUSTOM_KEY_PARAM_T * cur_custom_key_ptr = PNULL;

    if (PNULL == im_ptr)
    {
        return;
    }

    //清空定制按键列表
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
Description: 判定该msg是否引发切换至按键输入
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 任何输入区的按键都将会引发切换
输入区，即简单的定义为连在一起的下面整个键盘区域
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
Description: 向待处理的消息循环队列增加一条消息
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 该函数是输入应用调用的，在处理系统消息的时候
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
Description: 在处理非系统消息的时候，提交的数据
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 该函数是输入应用调用的
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
Description: 获取输入法的显示图标
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
Description: 获取输入法的显示名称
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
Description: 获取输入法的显示区域，即用来显示ui的部分
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 注意和触笔消息获取区域的区别
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


//输入法限制设定部分
/*==============================================================================
Description: 会清空之前的设置，重新设置
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
Description: 重新设置特别指定的输入要求
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
Description: 清空当前输入法的输入缓存，即输入序列，进入初始化状态
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
返回值为 - 输入法控件是否显示了softkey
注意与 DisplaySoftkey() 的区别 - 这个函数的返回值为是否需要通知父控件刷新
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
Description: 当输入法作为非active控件启动时，可理解为输入序列和字符串的转换器
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
Note: 特定的应用，可以设置输入的按键。
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
Description: 设置输入法的附加属性
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 在设置tag之后，简单的重建输入法应用
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
Description: 检测是否可以从当前的输入方式切换至键盘输入
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

