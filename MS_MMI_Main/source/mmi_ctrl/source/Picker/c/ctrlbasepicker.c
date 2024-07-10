/*****************************************************************************
** File Name:      ctrlbasepicker.c                                               *
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
#include "mmk_tp.h"
#include "guires.h"
#include "mmi_theme.h"

#include "ctrlbasepicker.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define BASEPICKER_TIPS_SPACE           2
#define BASEPICKER_TIPS_OFFSET          15
#define BASEPICKER_MAX_DISP_COUNT       10              // һ����������ʾ����
#define BASEPICKER_TP_MOVE_DIS          5               // �ж��Ƿ��ƶ���ƫ����

#define BASEPICKER_FIELD_NONE           (-1)

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef struct
{
    GUI_RECT_T      disp_rect;      // ��ʾ���������þ���λ��
    uint16          value;          // ��ʾ��ֵ��AMPM�Լ�����
    GUI_FONT_ALL_T  font;           // ��ʾ������
} BASEPICKER_FIELD_INFO_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init basepicker class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlInitVtbl (
    CTRLBASEPICKER_VTBL_T        *basepicker_vtbl_ptr
);

/*****************************************************************************/
//  Description : basepicker construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLBASEPICKER_INIT_PARAM_T *basepicker_param_ptr
);

/*****************************************************************************/
//  Description : basepicker destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle basepicker msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BasePickerCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get basepicker pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLBASEPICKER_OBJ_T* GetBasePickerPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN BasePickerTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
);

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
);

/*****************************************************************************/
// 	Description : set ctrl rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlSetRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr       //in:
);

/*****************************************************************************/
//  Description : ��ȡ��ʼ���������(��������ֵ)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerCtrlGetFieldRange (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  *start_field_ptr,       // [out]
    int32                  *end_field_ptr          // [out]
);

/*****************************************************************************/
//  Description : ��ȡָ�������ֵ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerCtrlGetValueRange (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,// [in]
    CTRLBASEPICKER_FIELD_T      field,               // [in]
    uint16                      *min_value_ptr,      // [out]
    uint16                      *max_value_ptr       // [out]
);

/*****************************************************************************/
//  Description : ��ȡָ����������ʾλ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T BasePickerCtrlGetFieldRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    int32                   field                   // [in]
);

/*****************************************************************************/
//  Description : ��ȡ���е�����
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerCtrlGetFieldText (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      cur_index,              // [in]
    MMI_STRING_T                *dis_string_ptr         // [out]
);

/*****************************************************************************/
//  Description : ��ȡ��ʾ�ı�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T BasePickerCtrlGetFieldTips (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,     // [in]
    int32                  field               // [in]
);

/*****************************************************************************/
//  Description : ����TP down��Ϣ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BasePickerHandleTpDown (
    CTRLBASEPICKER_OBJ_T  *basepicker_ctrl_ptr,   // [in]
    DPARAM                param                   // [in]
);

/*****************************************************************************/
//  Description : ����TP move��Ϣ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BasePickerHandleTpMove (
    CTRLBASEPICKER_OBJ_T  *basepicker_ctrl_ptr,    // [in]
    DPARAM                param              // [in]
);

/*****************************************************************************/
//  Description : ����TP up��Ϣ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BasePickerHandleTpUp (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    DPARAM                  param                   // [in]
);

/*****************************************************************************/
//  Description : ����scroll״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStartTpScrollTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr  // [in]
);

/*****************************************************************************/
//  Description : �ر�scroll״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStopTpScrollTimer (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T        field                   // [in] ָ���ĸ���ģ�����������������ã��Ͳ���ֹͣ
);

/*****************************************************************************/
//  Description : ����scroll״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//   scroll��ʱ���������ж�ÿ������Ϊֻ�п���һ�������scroll״̬
/*****************************************************************************/
LOCAL void BasePickerHandleTpScrollTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr  // [in]
);

/*****************************************************************************/
//  Description : ����fling״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStartFlingTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr     // [in]
);

/*****************************************************************************/
//  Description : �ر�fling״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStopFlingTimer (
    CTRLBASEPICKER_OBJ_T         *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T       field                   // [in] ָ���ĸ���ģ�����������������ã��Ͳ���ֹͣ
);

/*****************************************************************************/
//  Description : ����fling״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerHandleFlingTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr     // [in]
);

/*****************************************************************************/
//  Description : ����recover״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStartRecoverTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr       // [in]
);

/*****************************************************************************/
//  Description : �ر�recover״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStopRecoverTimer (
    CTRLBASEPICKER_OBJ_T           *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T         field                   // [in] ָ���ĸ���ģ�����������������ã��Ͳ���ֹͣ
);

/*****************************************************************************/
//  Description : ����ָ�״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerHandleRecoverTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr   // [in]
);

/*****************************************************************************/
//  Description : ����ĳ���������
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayField (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T        field,                  // [in]
    GUI_RECT_T                    *dis_rect_ptr           // [in] ��ʾ����
);

/*****************************************************************************/
//  Description : ���Ʊ���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayBg (
    CTRLBASEPICKER_OBJ_T      *basepicker_ctrl_ptr,     // [in]
    GUI_RECT_T                *dis_rect_ptr,
    GUI_RECT_T                *img_rect_ptr
);

/*****************************************************************************/
//  Description : ���Ʊ߿�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayBorder (
    CTRLBASEPICKER_OBJ_T  *basepicker_ctrl_ptr,   // [in]
    GUI_RECT_T            *client_rect_ptr,       // [in]
    GUI_RECT_T            *img_rect_ptr           // [in]
);

/*****************************************************************************/
//  Description : ��ʾ�ı�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayText (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    GUI_RECT_T                  *disp_rect_ptr          // [in]
);

/*****************************************************************************/
//  Description : ���Ʒָ���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplaySeparator (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    GUI_RECT_T             *separator_rect_ptr,    // [in]
    GUI_RECT_T             *relative_rect_ptr,     // [in]
    GUI_POINT_T            *start_pt_ptr,          // [in]
    MMI_IMAGE_ID_T         separator_image_id      // [in]
);

/*****************************************************************************/
//  Description : �����ɰ�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayMask (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,     // [in]
    GUI_RECT_T              *dis_rect_ptr
);

/*****************************************************************************/
//  Description : ������ʾ�ı�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayTips (
    CTRLBASEPICKER_OBJ_T            *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T          field,                  // [in]
    GUI_RECT_T                      *dis_rect_ptr,          // [in]
    GUI_RECT_T                      *clip_rect_ptr          // [in]
);

/*****************************************************************************/
//  Description : ����ƫ��λ�ã����Զ����½���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetBasePickerOffset (
    CTRLBASEPICKER_OBJ_T           *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T         field,                  // [in]
    int32                          offset                  // [in] �뵱ǰֵ��ƫ����
);

/*****************************************************************************/
//  Description : ����������ֵ�Ƿ�Ϸ�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CheckValid (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T        field                   // [in]
);

/*****************************************************************************/
//  Description : ���ݶ��뷽ʽ����ȡ��ʾ�����λ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetStartPoint (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    GUI_POINT_T             *start_point_ptr        // [out]
);

/*****************************************************************************/
//  Description : ���ÿ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void SetBasePickerSize (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,         // [in]
    uint16                 width,                  // [in]
    uint16                 height                  // [in]
);

/*****************************************************************************/
//  Description : ��ȡ���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GetBasePickerSize (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    uint16                 *width_ptr,             // [out]
    uint16                 *height_ptr             // [out]
);

/*****************************************************************************/
//  Description : ��ȡ�о�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TranslateRect (
    GUI_RECT_T     *rect_ptr,      // [in/out]
    int16          x,              // [in]
    int16          y               // [in]
);

/*****************************************************************************/
//  Description : ��ȡλ�ú�������Ϣ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetBasePickerInfo (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,     // [in]
    CTRLBASEPICKER_FIELD_T      field,              // [in]
    BASEPICKER_FIELD_INFO_T     *field_info_ptr,    // [out] ���ص������Ǿ�������
    int32                       field_info_count    // [in]
);

/*****************************************************************************/
//  Description : ���ݵ�ǰֵ����ȡ��һ��������һ��ֵ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//  �����Ƿ�ɹ�������ǵ���ģ���ͷ֮�󷵻�FALSE
/*****************************************************************************/
LOCAL BOOLEAN GetNextValue (
    CTRLBASEPICKER_OBJ_T         *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T       field,                  // [in]
    uint16                       cur_date_value,         // [in]
    BOOLEAN                      is_next,                // [in]
    uint16                       *next_value_ptr         // [out]
);

/*****************************************************************************/
//  Description : ����ѡ����Ϣ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL void PostPickerSelectedMsg (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr     // [in]
);

/*****************************************************************************/
//  Description : ��ȡitem�ĸ߶�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL uint16 GetItemHeight (
    CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr  // [in]
);

/*****************************************************************************/
//  Description : get align
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
__inline LOCAL GUI_ALIGN_E GetAlign (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr
);

/*****************************************************************************/
//  Description : ����DOWNʱ�ĵ㣬�ж��Ƿ��ƶ���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL BOOLEAN IsTpMoved (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    GUI_POINT_T            *point_ptr              // [in]
);
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get basepicker type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASEPICKER_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                       // parent class type
                       "basepicker",                                         // control name
                       sizeof (CTRLBASEPICKER_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) BasePickerCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) BasePickerCtrlDestruct,      // control destructor function
                       sizeof (CTRLBASEPICKER_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) PNULL,                           // convert init param
                       sizeof (CTRLBASEPICKER_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) BasePickerCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init basepicker class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlInitVtbl (
    CTRLBASEPICKER_VTBL_T        *basepicker_vtbl_ptr
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) basepicker_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = BasePickerCtrlHandleMsg;

    // ���½ӿ�����ʵ����Ƕform
    base_ctrl_vtbl_ptr->GetHeightByWidth = BasePickerCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->SetDisplayRect = BasePickerCtrlSetDisplayRect;
    base_ctrl_vtbl_ptr->SetRect = BasePickerCtrlSetRect;

    // virtual method for base picker ctrl
    basepicker_vtbl_ptr->GetFieldRange = BasePickerCtrlGetFieldRange;
    basepicker_vtbl_ptr->GetValueRange = BasePickerCtrlGetValueRange;
    basepicker_vtbl_ptr->GetFieldRect = BasePickerCtrlGetFieldRect;
    basepicker_vtbl_ptr->GetFieldText = BasePickerCtrlGetFieldText;
    basepicker_vtbl_ptr->GetFieldTips = BasePickerCtrlGetFieldTips;
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : basepicker construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLBASEPICKER_INIT_PARAM_T *basepicker_param_ptr
)
{
    BOOLEAN                 result = TRUE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == basepicker_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetBasePickerTheme (&basepicker_ctrl_ptr->theme);

    base_ctrl_ptr->bg = basepicker_ctrl_ptr->theme.bg;

    BASEPICKER_Init(basepicker_ctrl_ptr);

#endif

    return (result);
}

/*****************************************************************************/
//  Description : basepicker destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN                 result = TRUE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;

    if (PNULL == basepicker_ctrl_ptr)
    {
        return FALSE;
    }

    BASEPICKER_Destroy(basepicker_ctrl_ptr);

#endif

    return (result);
}

/*****************************************************************************/
//  Description : ������ʾ���ֵ�λ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetTipsPos (
    MMI_CTRL_ID_T                 ctrl_id,
    CTRLBASEPICKER_TIPS_POS_E     tips_pos
)
{
    BOOLEAN         result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    basepicker_ctrl_ptr = GetBasePickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr)
    {
        BASEPICKER_SetTipsPos (basepicker_ctrl_ptr, tips_pos);

        result = TRUE;
    }

#endif

    return result;
}

/*****************************************************************************/
//  Description : �Ƿ�������ֵ��ʱ��������
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_EnableAnim (
    MMI_CTRL_ID_T         ctrl_id,
    BOOLEAN               enable_anim
)
{
    BOOLEAN         result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    basepicker_ctrl_ptr = GetBasePickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr)
    {
        basepicker_ctrl_ptr->enable_anim = enable_anim;
        result = TRUE;
    }

#endif

    return result;
}

/*****************************************************************************/
//  Description : ����picker���ĸ߶�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetHeight (
    MMI_CTRL_ID_T        ctrl_id,
    uint16               height
)
{
    BOOLEAN         result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    basepicker_ctrl_ptr = GetBasePickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr && 0 != height)
    {
        basepicker_ctrl_ptr->theme.client_height = height;
        result = TRUE;
    }

#endif

    return result;
}

/*****************************************************************************/
//  Description : ����picker�����и�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetLineHeight (
    MMI_CTRL_ID_T    ctrl_id,
    uint16           line_height
)
{
    BOOLEAN                 result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    uint16                  value[MMITHEME_BASEPICKER_MAX_FIELD_COUNT] = {0};
    int32                   i = 0;
    int32                   start_field = 0;
    int32                   end_field = 0;
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    basepicker_ctrl_ptr = GetBasePickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr)
    {
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        // store the value of each field, because it is calculated by line height
        for (i = start_field; i < end_field; i++)
        {
            value[i-start_field] = BASEPICKER_GetValue (basepicker_ctrl_ptr, i);
        }

        basepicker_ctrl_ptr->theme.line_height = line_height;

        // set the value again by new line height
        for (i = start_field; i < end_field; i++)
        {
            BASEPICKER_SetValue (basepicker_ctrl_ptr, i, value[i-start_field]);
        }

        result = TRUE;
    }

#endif

    return result;
}

/*****************************************************************************/
//  Description : ����picker������������
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetFont (
    MMI_CTRL_ID_T      ctrl_id,
    GUI_FONT_ALL_T     *normal_font_ptr,
    GUI_FONT_ALL_T     *active_font_ptr
)
{
    BOOLEAN         result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    basepicker_ctrl_ptr = GetBasePickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr)
    {
        if (PNULL != normal_font_ptr)
        {
            basepicker_ctrl_ptr->theme.content_font = *normal_font_ptr;
        }

        if (PNULL != active_font_ptr)
        {
            basepicker_ctrl_ptr->theme.cur_content_font = *active_font_ptr;
        }

        result = TRUE;
    }

#endif

    return result;
}

#ifdef TOUCH_PANEL_SUPPORT

/*****************************************************************************/
//  Description : init
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Init (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr    // [in]
)
{
    int32                   i = 0;
    int32                   start_field = 0;
    int32                   end_field   = 0;

    basepicker_ctrl_ptr->tp_down_field = BASEPICKER_FIELD_NONE;

    // ��ȡ��ǰ����£���Ŀ�ʼ�ͽ�����Χ�����з����ͳһ����
    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

    for (i = start_field; i < end_field; i++)
    {
        // ���㣬������������ó���Сֵ
        BASEPICKER_SetValue (basepicker_ctrl_ptr, i, 0);

        SCI_ASSERT (PNULL == basepicker_ctrl_ptr->state[i].tracker_ptr);   /*assert verified*/
        basepicker_ctrl_ptr->state[i].tracker_ptr = MMK_CreateTracker();   // ����������
        basepicker_ctrl_ptr->state[i].is_loop = TRUE;                      // Ĭ�϶�Ҫѭ������
    }
}

/*****************************************************************************/
//  Description : destroy
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Destroy (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr    // [in]
)
{
    int32 i = 0;

    BASEPICKER_Reset (basepicker_ctrl_ptr);

    for (i = 0; i < MMITHEME_BASEPICKER_MAX_FIELD_COUNT; i++)
    {
        if (PNULL != basepicker_ctrl_ptr->state[i].tracker_ptr)
        {
            MMK_DestroyTracker (basepicker_ctrl_ptr->state[i].tracker_ptr);
            basepicker_ctrl_ptr->state[i].tracker_ptr = PNULL;
        }
    }
}

/*****************************************************************************/
//  Description : ���������ֵ�����������ֵ���Զ�����ֵ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEPICKER_SetValue (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      value                   // [in]
)
{
    BOOLEAN result = TRUE;
    uint16  item_height = 0;
    uint16  min_value = 0;
    uint16  max_value = 0;

    VTLBASEPICKER_GetValueRange (basepicker_ctrl_ptr, field, &min_value, &max_value);

    // ������ֵ����Ϊ��ֵ
    if (value < min_value)
    {
        value = min_value;
    }
    else if (value > max_value)
    {
        value = max_value;
    }

    // height of a item
    item_height = GetItemHeight (basepicker_ctrl_ptr);

    basepicker_ctrl_ptr->state[field].cur_offset = (value - min_value) * item_height;

    CheckValid (basepicker_ctrl_ptr, field);

    return result;
}

/*****************************************************************************/
//  Description : ��ȡ�����ֵ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEPICKER_GetValue (
    CTRLBASEPICKER_OBJ_T         *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T       field                   // [in]
)
{
    uint16  min_value = 0;
    uint16  max_value = 0;
    int32   cur_index = 0;
    uint16  item_height = 0;

    VTLBASEPICKER_GetValueRange (basepicker_ctrl_ptr, field, &min_value, &max_value);

    // height of a item
    item_height = GetItemHeight (basepicker_ctrl_ptr);

    cur_index = basepicker_ctrl_ptr->state[field].cur_offset / item_height;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(cur_index >= 0);     /*assert verified*/
    if (cur_index >= 0)
    {
        min_value += cur_index;
    }

    return min_value;
}

/*****************************************************************************/
//  Description : ����Ŀ��ֵ��������recover״̬
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_SetRecoverValue (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      value,                  // [in],
    BOOLEAN                     is_up                   // [in] �����Ƿ�����
)
{
    uint16  min_value = 0;
    uint16  max_value = 0;
    uint16  item_height = 0;

    // height of a item
    item_height = GetItemHeight (basepicker_ctrl_ptr);

    VTLBASEPICKER_GetValueRange (basepicker_ctrl_ptr, field, &min_value, &max_value);

    basepicker_ctrl_ptr->state[field].recover_cur_offset = basepicker_ctrl_ptr->state[field].cur_offset;
    basepicker_ctrl_ptr->state[field].recover_dst_offset = item_height * (value - min_value);

    if (basepicker_ctrl_ptr->state[field].recover_cur_offset == basepicker_ctrl_ptr->state[field].recover_dst_offset)
    {
        basepicker_ctrl_ptr->state[field].recover_move_step = 0;
    }
    else if (is_up)
    {
        // recover_dst_offset��ֵ���Գ����ܳ���
        if (basepicker_ctrl_ptr->state[field].recover_dst_offset < basepicker_ctrl_ptr->state[field].recover_cur_offset)
        {
            basepicker_ctrl_ptr->state[field].recover_dst_offset += item_height * (max_value - min_value + 1);
        }

        // �����ӱ�
        basepicker_ctrl_ptr->state[field].recover_move_step = (2 * MMITHEME_BASEPICKER_RECOVER_DISTANCE);
    }
    else
    {
        // recover_dst_offset��ֵ����<0
        if (basepicker_ctrl_ptr->state[field].recover_dst_offset > basepicker_ctrl_ptr->state[field].recover_cur_offset)
        {
            basepicker_ctrl_ptr->state[field].recover_dst_offset -= item_height * (max_value - min_value + 1);
        }

        // �����ӱ�
        basepicker_ctrl_ptr->state[field].recover_move_step = - (2 * MMITHEME_BASEPICKER_RECOVER_DISTANCE);
    }

    basepicker_ctrl_ptr->state[field].slide_state = MMK_TP_SLIDE_RECOVER;
    BasePickerStartRecoverTimer (basepicker_ctrl_ptr);
}

/*****************************************************************************/
//  Description : reset picker
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Reset (
    CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr
)
{
    int32           i = 0;
    int32           start_field = 0;
    int32           end_field = 0;
    uint16          value = 0;

    // reset the value
    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

    for (i = start_field; i < end_field; i++)
    {
        basepicker_ctrl_ptr->state[i].slide_state = MMK_TP_SLIDE_NONE;

        value = BASEPICKER_GetValue (basepicker_ctrl_ptr, i);
        BASEPICKER_SetValue (basepicker_ctrl_ptr, i, value);
    }

    basepicker_ctrl_ptr->tp_down_field = BASEPICKER_FIELD_NONE;

    // reset all timers
    if (0 != basepicker_ctrl_ptr->tpscroll_timer_id)
    {
        MMK_StopTimer (basepicker_ctrl_ptr->tpscroll_timer_id);
        basepicker_ctrl_ptr->tpscroll_timer_id = 0;
    }

    if (0 != basepicker_ctrl_ptr->fling_timer_id)
    {
        MMK_StopTimer (basepicker_ctrl_ptr->fling_timer_id);
        basepicker_ctrl_ptr->fling_timer_id = 0;
    }

    if (0 != basepicker_ctrl_ptr->recover_timer_id)
    {
        MMK_StopTimer (basepicker_ctrl_ptr->recover_timer_id);
        basepicker_ctrl_ptr->recover_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : ����������ʾpicker����
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Display (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr    // [in]
)
{
    BOOLEAN             result = FALSE;
    uint16              client_width = 0;
    uint16              client_height = 0;
    int32               i = 0;
    int32               start_field = 0;
    int32               end_field = 0;
    GUI_RECT_T          cross_rect = {0};
    GUI_RECT_T          client_rect = {0};              // ͼƬ����ʾ����(��������)
    GUI_RECT_T          relative_dis_rect = {0};        // ��ʾ������������(�����ͼƬ�������)
    GUI_RECT_T          text_rect = {0};                // �����ж��ı��Ƿ��ڿ�������
    GUI_RECT_T          tips_clip_rect = {0};
    GUI_RECT_T          field_rect = {0};
    GUI_RECT_T          separator_rect = {0};
    GUI_POINT_T         start_point = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    if (!GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, base_ctrl_ptr->rect))
    {
        return;
    }

    // ��ȡ��ʾ�����λ�ã���ͼƬ�Ŀ����Ϣ
    result = GetStartPoint (basepicker_ctrl_ptr, &start_point);

    if (!result)
    {
        return;
    }

    GetBasePickerSize (basepicker_ctrl_ptr, &client_width, &client_height);

    // ͼƬ������(ת��Ϊ����λ��)
    client_rect.left = start_point.x;
    client_rect.right = client_rect.left + client_width - 1;
    client_rect.top = start_point.y;
    client_rect.bottom = client_rect.top + client_height - 1;

    // �ܹ���ʾ�����
    if (GUI_IntersectRect (&client_rect, client_rect, cross_rect))
    {
        // ��Ҫ���Ƶ��������
        relative_dis_rect = client_rect;
        TranslateRect (&relative_dis_rect, -start_point.x, -start_point.y);

        // ������ͼ
        BasePickerDisplayBg (basepicker_ctrl_ptr, &client_rect, &relative_dis_rect);

        // ���ɲ�
        // picker����ɰ�������ֵ���
        if (!basepicker_ctrl_ptr->theme.is_mask_on_top)
        {
            BasePickerDisplayMask (basepicker_ctrl_ptr, &cross_rect);
        }

        // ������, ��ʾ���֣��ָ���
        {
            VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

            separator_rect.top = 0;
            separator_rect.bottom = separator_rect.top + client_height - 1;

            // ��������Ƿ�����ʾ������
            for (i = start_field; i < end_field; i++)
            {
                field_rect = VTLBASEPICKER_GetFieldRect (basepicker_ctrl_ptr, i);

                // ������
                if (GUI_IntersectRect (&text_rect, field_rect, relative_dis_rect))
                {
                    TranslateRect (&text_rect, start_point.x, start_point.y);
                    BasePickerDisplayText (basepicker_ctrl_ptr, i, &text_rect);
                }

                // ����ʾ�ı�
                if (CTRLBASEPICKER_TIPS_POS_TOP == basepicker_ctrl_ptr->tips_pos)
                {
                    text_rect = field_rect;
                    text_rect.top = 0;
                    text_rect.bottom = field_rect.top - basepicker_ctrl_ptr->theme.border_width - 1;

                    if (GUI_IntersectRect (&tips_clip_rect, text_rect, relative_dis_rect))
                    {
                        TranslateRect (&text_rect, start_point.x, start_point.y);
                        TranslateRect (&tips_clip_rect, start_point.x, start_point.y);

                        BasePickerDisplayTips (basepicker_ctrl_ptr, i, &text_rect, &tips_clip_rect);
                    }
                }

                // ���ָ���
                if (basepicker_ctrl_ptr->theme.separator_width > 0)
                {
                    if (i == start_field)           // �����1���������ߵķָ���
                    {
                        separator_rect.right = field_rect.left - 1;
                        separator_rect.left = separator_rect.right - basepicker_ctrl_ptr->theme.separator_width + 1;

                        BasePickerDisplaySeparator (
                            basepicker_ctrl_ptr, &separator_rect, &relative_dis_rect,
                            &start_point, basepicker_ctrl_ptr->theme.separator_image_id);
                    }

                    separator_rect.left = field_rect.right + 1;
                    separator_rect.right = separator_rect.left + basepicker_ctrl_ptr->theme.separator_width - 1;

                    BasePickerDisplaySeparator (
                        basepicker_ctrl_ptr, &separator_rect, &relative_dis_rect,
                        &start_point, basepicker_ctrl_ptr->theme.separator_image_id);
                }
            }
        }

        // ���ɲ�
        // ��picker����ɰ�����������
        if (basepicker_ctrl_ptr->theme.is_mask_on_top)
        {
            BasePickerDisplayMask (basepicker_ctrl_ptr, &cross_rect);
        }

        // ���߿�
        if (basepicker_ctrl_ptr->theme.border_width > 0)
        {
            BasePickerDisplayBorder (basepicker_ctrl_ptr, &client_rect, &relative_dis_rect);
        }
    }
}

/*****************************************************************************/
//  Description : ������ʾ���ֵ�λ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEPICKER_SetTipsPos (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,
    CTRLBASEPICKER_TIPS_POS_E     tips_pos
)
{
    basepicker_ctrl_ptr->tips_pos = tips_pos;

    if (CTRLBASEPICKER_TIPS_POS_TOP != tips_pos)
    {
        basepicker_ctrl_ptr->theme.top_margin = basepicker_ctrl_ptr->theme.bottom_margin;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : ��ȡ�����ı�������(�۳���ʾ����)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEPICKER_GetTextRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr
)
{
    GUI_RECT_T  text_rect = {0};

    text_rect.top = basepicker_ctrl_ptr->theme.top_margin;
    text_rect.bottom = basepicker_ctrl_ptr->theme.client_height - basepicker_ctrl_ptr->theme.bottom_margin - 1;
    text_rect.left = basepicker_ctrl_ptr->theme.separator_width + basepicker_ctrl_ptr->theme.border_width;
    text_rect.right = basepicker_ctrl_ptr->theme.client_width - (basepicker_ctrl_ptr->theme.separator_width + basepicker_ctrl_ptr->theme.border_width) - 1;

    return text_rect;
}

/*****************************************************************************/
//  Description : handle basepicker msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BasePickerCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    CTRLBASE_VTBL_T         *parent_vtbl_ptr = PNULL;
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;

    if (PNULL == basepicker_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_PAINT:
        if (VTLBASE_GetVisible (base_ctrl_ptr))
        {
            BASEPICKER_Display (basepicker_ctrl_ptr);
        }

        break;

    case MSG_CTL_GET_ACTIVE:
        break;

    case MSG_CTL_LOSE_ACTIVE:
    case MSG_CTL_CLOSE:
        break;

    case MSG_CTL_LOSE_FOCUS:
        BASEPICKER_Reset (basepicker_ctrl_ptr);
        break;

    case MSG_NOTIFY_LOSE_ACTIVE:
        break;

    case MSG_TIMER:

        if (basepicker_ctrl_ptr->tpscroll_timer_id == * (uint8 *) param)
        {
            BasePickerHandleTpScrollTimer (basepicker_ctrl_ptr);
        }
        else if (basepicker_ctrl_ptr->fling_timer_id == * (uint8 *) param)
        {
            BasePickerHandleFlingTimer (basepicker_ctrl_ptr);
        }
        else if (basepicker_ctrl_ptr->recover_timer_id == * (uint8 *) param)
        {
            BasePickerHandleRecoverTimer (basepicker_ctrl_ptr);
        }

        break;

    case MSG_TP_PRESS_DOWN:
        BasePickerHandleTpDown (basepicker_ctrl_ptr, param);
        break;

    case MSG_TP_PRESS_MOVE:
        BasePickerHandleTpMove (basepicker_ctrl_ptr, param);
        break;

    case MSG_TP_PRESS_UP:
        BasePickerHandleTpUp (basepicker_ctrl_ptr, param);
        break;

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEPICKER_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
)
{
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;

    SetBasePickerSize (basepicker_ctrl_ptr, width, 0);
    GetBasePickerSize (basepicker_ctrl_ptr, PNULL, height_ptr);

    return TRUE;
}

/*****************************************************************************/
// 	Description : set ctrl rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlSetRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr       //in:
)
{
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T        *parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEPICKER_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);

    if ( (!GUI_IsRectEmpty (*rect_ptr)) &&
            (!GUI_EqualRect (base_ctrl_ptr->rect, *rect_ptr)))
    {
        parent_vtbl_ptr->SetRect (base_ctrl_ptr, rect_ptr);

        GUI_SetVOrHRect( base_ctrl_ptr->handle, (GUI_RECT_T *)rect_ptr,&(base_ctrl_ptr->both_rect));

        SetBasePickerSize (
            basepicker_ctrl_ptr,
            rect_ptr->right - rect_ptr->left + 1,
            rect_ptr->bottom - rect_ptr->top + 1);
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
)
{
    BOOLEAN     result = FALSE;
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T        *parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEPICKER_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);

    result = parent_vtbl_ptr->SetDisplayRect (base_ctrl_ptr, rect_ptr, is_update);

    if (is_update)
    {
        BASEPICKER_Display (basepicker_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : ��ȡ��ʼ���������(��������ֵ)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerCtrlGetFieldRange (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  *start_field_ptr,       // [out]
    int32                  *end_field_ptr          // [out]
)
{
    SCI_ASSERT (0); /*assert verified*/
}

/*****************************************************************************/
//  Description : ��ȡָ�������ֵ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerCtrlGetValueRange (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,// [in]
    CTRLBASEPICKER_FIELD_T      field,               // [in]
    uint16                      *min_value_ptr,      // [out]
    uint16                      *max_value_ptr       // [out]
)
{
    SCI_ASSERT (0); /*assert verified*/
}

/*****************************************************************************/
//  Description : ��ȡָ����������ʾλ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T BasePickerCtrlGetFieldRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    int32                   field                   // [in]
)
{
    GUI_RECT_T temp = {0};

    SCI_ASSERT (0); /*assert verified*/
    return temp;
}

/*****************************************************************************/
//  Description : ��ȡ���е�����
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerCtrlGetFieldText (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      cur_index,              // [in]
    MMI_STRING_T                *dis_string_ptr         // [out]
)
{
    SCI_ASSERT (0); /*assert verified*/
}

/*****************************************************************************/
//  Description : ��ȡ��ʾ�ı�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T BasePickerCtrlGetFieldTips (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,     // [in]
    int32                  field               // [in]
)
{
    MMI_TEXT_ID_T temp = 0;
    SCI_ASSERT (0); /*assert verified*/
    return temp;
}

/*****************************************************************************/
//  Description : ����TP down��Ϣ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BasePickerHandleTpDown (
    CTRLBASEPICKER_OBJ_T  *basepicker_ctrl_ptr,   // [in]
    DPARAM                param                   // [in]
)
{
    int32           i = 0;
    int32           start_field = 0;
    int32           end_field = 0;
    GUI_POINT_T     tp_point = {0};             // TP���λ��
    GUI_POINT_T     relative_tp_point = {0};    // TP����ڿؼ���λ��
    GUI_POINT_T     start_point = {0};          // ���Ƶ���ʼλ��

    //set tp press point
    tp_point.x = MMK_GET_TP_X (param);
    tp_point.y = MMK_GET_TP_Y (param);

    basepicker_ctrl_ptr->tp_down_field = BASEPICKER_FIELD_NONE;

    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

    GetStartPoint (basepicker_ctrl_ptr, &start_point);

    for (i = start_field; i < end_field; i++)
    {
        // ����TP���λ��
        relative_tp_point = tp_point;
        relative_tp_point.x -= start_point.x;
        relative_tp_point.y -= start_point.y;

        // �Ƿ������������
        if (GUI_PointIsInRect (relative_tp_point, VTLBASEPICKER_GetFieldRect (basepicker_ctrl_ptr, i)))
        {
            // ֻ������MMK_TP_SLIDE_FLING,MMK_TP_SLIDE_RECOVER,MMK_TP_SLIDE_NONE
            if (MMK_TP_SLIDE_TP_SCROLL == basepicker_ctrl_ptr->state[i].slide_state)
            {
                //SCI_TRACE_LOW:"slide_state error: %d"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TOUCH_645_112_2_18_3_16_22_138, (uint8*) "d", basepicker_ctrl_ptr->state[i].slide_state);
                break;
            }

            basepicker_ctrl_ptr->is_tp_moved = FALSE;

            basepicker_ctrl_ptr->tp_down_field = i;

            // ��¼��ʼ��
            basepicker_ctrl_ptr->tp_start_point = tp_point;

            // ��¼ǰһ����
            basepicker_ctrl_ptr->tp_pre_point = tp_point;

            MMK_ResetVelocityItemByTracker (
                basepicker_ctrl_ptr->state[basepicker_ctrl_ptr->tp_down_field].tracker_ptr);

            MMK_AddVelocityItemByTracker (
                basepicker_ctrl_ptr->state[i].tracker_ptr,
                tp_point.x, tp_point.y);

            // ���϶���ָ������У�TP����ȥ�ĵ㣬�����ƶ���������TP UP�Ͳ��Ὣ�����ֵ�ƶ�
            if (MMK_TP_SLIDE_FLING == basepicker_ctrl_ptr->state[i].slide_state)
            {
                basepicker_ctrl_ptr->is_tp_moved = TRUE;
                BasePickerStopFlingTimer (basepicker_ctrl_ptr, i);
            }
            else if (MMK_TP_SLIDE_RECOVER == basepicker_ctrl_ptr->state[i].slide_state)
            {
                basepicker_ctrl_ptr->is_tp_moved = TRUE;
                BasePickerStopRecoverTimer (basepicker_ctrl_ptr, i);
            }

            basepicker_ctrl_ptr->state[i].slide_state = MMK_TP_SLIDE_NONE;

            break;
        }
    }
}

/*****************************************************************************/
//  Description : ����TP move��Ϣ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BasePickerHandleTpMove (
    CTRLBASEPICKER_OBJ_T  *basepicker_ctrl_ptr,    // [in]
    DPARAM                param              // [in]
)
{
    GUI_POINT_T             tp_point = {0};
    CTRLBASEPICKER_FIELD_T  field = basepicker_ctrl_ptr->tp_down_field;    // tp_down_field�����õ�̫���ˣ���������ʱ������һ��

    if (BASEPICKER_FIELD_NONE != field)
    {
        if (MMK_TP_SLIDE_NONE != basepicker_ctrl_ptr->state[field].slide_state
                && MMK_TP_SLIDE_TP_SCROLL != basepicker_ctrl_ptr->state[field].slide_state)
        {
            //SCI_TRACE_LOW:"slide_state error: %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TOUCH_702_112_2_18_3_16_22_139, (uint8*) "d", basepicker_ctrl_ptr->state[field].slide_state);
            return;
        }

        basepicker_ctrl_ptr->is_tp_moved = TRUE;

        //set tp press point
        tp_point.x = MMK_GET_TP_X (param);
        tp_point.y = MMK_GET_TP_Y (param);

        // �����ٶȵ�
        MMK_AddVelocityItemByTracker (
            basepicker_ctrl_ptr->state[field].tracker_ptr,
            tp_point.x, tp_point.y);

        if (MMK_TP_SLIDE_TP_SCROLL != basepicker_ctrl_ptr->state[field].slide_state
                && IsTpMoved (basepicker_ctrl_ptr, &tp_point))
        {
            basepicker_ctrl_ptr->state[field].slide_state = MMK_TP_SLIDE_TP_SCROLL;
            BasePickerStartTpScrollTimer (basepicker_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : ����TP up��Ϣ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BasePickerHandleTpUp (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    DPARAM                  param                   // [in]
)
{
    int32           i = 0;
    uint16          item_height = 0;
    int32           item_offset = 0;
    GUI_POINT_T     tp_point = {0};
    CTRLBASEPICKER_FIELD_T      field = basepicker_ctrl_ptr->tp_down_field;
    BASEPICKER_FIELD_INFO_T     field_info[BASEPICKER_MAX_DISP_COUNT] = {0};     // ������ֵ����ʾ��Ϣ(����λ�ú�ֵ)

    if (BASEPICKER_FIELD_NONE != field)
    {
        if (MMK_TP_SLIDE_NONE != basepicker_ctrl_ptr->state[field].slide_state
                && MMK_TP_SLIDE_TP_SCROLL != basepicker_ctrl_ptr->state[field].slide_state)
        {
            //SCI_TRACE_LOW:"slide_state error: %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TOUCH_747_112_2_18_3_16_22_140, (uint8*) "d", basepicker_ctrl_ptr->state[field].slide_state);
            return;
        }

        BasePickerStopTpScrollTimer (basepicker_ctrl_ptr, field);

        //set tp press point
        tp_point.x = MMK_GET_TP_X (param);
        tp_point.y = MMK_GET_TP_Y (param);

        // �����ٶȵ�
        MMK_AddVelocityItemByTracker (
            basepicker_ctrl_ptr->state[field].tracker_ptr,
            tp_point.x, tp_point.y);

        MMK_ComputeCurrentVelocityByTracker (
            basepicker_ctrl_ptr->state[field].tracker_ptr,
            (float *) PNULL,
            &basepicker_ctrl_ptr->state[field].fling_velocity,
            0.f,
            FLING_MAX_VELOCITY);

        //// ���ｵ���٣���Ȼ̫����
        //basepicker_ctrl_ptr->state[field].fling_velocity /= 2;

        if (FLING_MIN_VELOCITY <= abs ( (int32) basepicker_ctrl_ptr->state[field].fling_velocity))  // �ﵽһ���ٶȣ���Ҫ����
        {
            // ���뻬��״̬���𶯻�����ʱ��
            basepicker_ctrl_ptr->state[field].slide_state = MMK_TP_SLIDE_FLING;
            BasePickerStartFlingTimer (basepicker_ctrl_ptr);
        }
        else    // �ָ�ԭλ
        {
            // ���û���ƶ�����˵����ֱ�ӵ��ĳ������ģ��Զ������Ǹ�ֵ
            if (!basepicker_ctrl_ptr->is_tp_moved)
            {
                // ��ȡ��ʾ��Ϣ
                GetBasePickerInfo (basepicker_ctrl_ptr, field, field_info, BASEPICKER_MAX_DISP_COUNT);

                for (i = 0; i < BASEPICKER_MAX_DISP_COUNT; i++)
                {
                    if (GUI_PointIsInRect (tp_point, field_info[i].disp_rect))
                    {
                        if (field_info[i].disp_rect.top > field_info[0].disp_rect.top)  // �����·��������ƶ�
                        {
                            BASEPICKER_SetRecoverValue (basepicker_ctrl_ptr, field, field_info[i].value, TRUE);
                        }
                        else
                        {
                            BASEPICKER_SetRecoverValue (basepicker_ctrl_ptr, field, field_info[i].value, FALSE);
                        }

                        break;
                    }
                }

                // ��̧�𣬲���û���ƶ�������£�����Ϣ֪ͨӦ��
                PostPickerSelectedMsg (basepicker_ctrl_ptr);
            }
            else
            {
                // height of a item
                item_height = GetItemHeight (basepicker_ctrl_ptr);

                // ���item����������ƫ����
                item_offset = basepicker_ctrl_ptr->state[field].cur_offset % item_height;

                // ����Ŀ��λ�ã�����ǰ��λ��
                basepicker_ctrl_ptr->state[field].recover_cur_offset = basepicker_ctrl_ptr->state[field].cur_offset;
                basepicker_ctrl_ptr->state[field].recover_dst_offset = basepicker_ctrl_ptr->state[field].cur_offset - item_offset;

                // ȷ���ƶ�����
                if (0 == item_offset)   // ����ͣ�ڵ�ǰֵ��λ�ã���recover timer����timer��ʱ���ʱ������ֵ�Ƿ���ȷ
                {
                    basepicker_ctrl_ptr->state[field].recover_move_step = 0;
                }
                else if (item_offset > item_height / 2)       // �����ǰֵ�Ѿ��϶�������һ�룬������������һ��ֵ
                {
                    basepicker_ctrl_ptr->state[field].recover_move_step = MMITHEME_BASEPICKER_RECOVER_DISTANCE;
                    basepicker_ctrl_ptr->state[field].recover_dst_offset += item_height;
                }
                else    // ���򣬻ָ�����ǰֵ
                {
                    basepicker_ctrl_ptr->state[field].recover_move_step = -MMITHEME_BASEPICKER_RECOVER_DISTANCE;
                }

                basepicker_ctrl_ptr->state[field].slide_state = MMK_TP_SLIDE_RECOVER;
                BasePickerStartRecoverTimer (basepicker_ctrl_ptr);
            }
        }

        basepicker_ctrl_ptr->tp_down_field = BASEPICKER_FIELD_NONE;
    }
}

/*****************************************************************************/
//  Description : ����scroll״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStartTpScrollTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr  // [in]
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    if (PNULL != basepicker_ctrl_ptr && 0 == basepicker_ctrl_ptr->tpscroll_timer_id)
    {
        basepicker_ctrl_ptr->tpscroll_timer_id = MMK_CreateWinTimer (
                    base_ctrl_ptr->handle,
                    MMITHEME_BASEPICKER_TPSCROLL_TIME,
                    TRUE);
    }
}

/*****************************************************************************/
//  Description : �ر�scroll״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStopTpScrollTimer (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T        field                   // [in] ָ���ĸ���ģ�����������������ã��Ͳ���ֹͣ
)
{
    BOOLEAN can_stop = TRUE;
    int32   i = 0;
    int32   start_field = 0;
    int32   end_field = 0;

    if (PNULL != basepicker_ctrl_ptr && 0 != basepicker_ctrl_ptr->tpscroll_timer_id)
    {
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        // ��Ϊ�п��ܻ��ж����ͬʱ�����������ʱ��������Ҫ�жϸ������Ƿ����ã�
        // �پ����Ƿ�Ҫͣ��
        for (i = start_field; i < end_field; i++)
        {
            if (MMK_TP_SLIDE_TP_SCROLL == basepicker_ctrl_ptr->state[i].slide_state && i != field)
            {
                can_stop = FALSE;
                break;
            }
        }

        if (can_stop)
        {
            MMK_StopTimer (basepicker_ctrl_ptr->tpscroll_timer_id);
            basepicker_ctrl_ptr->tpscroll_timer_id = 0;
        }
    }
}

/*****************************************************************************/
//  Description : ����scroll״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//   scroll��ʱ���������ж�ÿ������Ϊֻ�п���һ�������scroll״̬
/*****************************************************************************/
LOCAL void BasePickerHandleTpScrollTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr  // [in]
)
{
    int32               tp_move_offset = 0;
    GUI_POINT_T         point   = {0};
    MMI_TP_STATUS_E     state   = MMI_TP_NONE;

    if (BASEPICKER_FIELD_NONE == basepicker_ctrl_ptr->tp_down_field)
    {
        //SCI_TRACE_LOW:"slide_state error: %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TOUCH_955_112_2_18_3_16_23_141, (uint8*) "d", basepicker_ctrl_ptr->tp_down_field);
        return;
    }

    // restart tp move timer
    BasePickerStopTpScrollTimer (basepicker_ctrl_ptr, basepicker_ctrl_ptr->tp_down_field);
    BasePickerStartTpScrollTimer (basepicker_ctrl_ptr);

    MMK_GetLogicTPMsg (&state, &point);

    tp_move_offset = basepicker_ctrl_ptr->tp_pre_point.y - point.y;

    SetBasePickerOffset (basepicker_ctrl_ptr, basepicker_ctrl_ptr->tp_down_field, tp_move_offset);

    // store tp point
    basepicker_ctrl_ptr->tp_pre_point = point;
}

/*****************************************************************************/
//  Description : ����fling״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStartFlingTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr     // [in]
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    if (PNULL != basepicker_ctrl_ptr && 0 == basepicker_ctrl_ptr->fling_timer_id)
    {
        basepicker_ctrl_ptr->fling_timer_id = MMK_CreateWinTimer (
                                                  base_ctrl_ptr->handle,
                                                  MMITHEME_BASEPICKER_FLING_TIME,
                                                  TRUE);
    }
}

/*****************************************************************************/
//  Description : �ر�fling״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStopFlingTimer (
    CTRLBASEPICKER_OBJ_T         *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T       field                   // [in] ָ���ĸ���ģ�����������������ã��Ͳ���ֹͣ
)
{
    BOOLEAN can_stop = TRUE;
    int32   i = 0;
    int32   start_field = 0;
    int32   end_field = 0;

    if (PNULL != basepicker_ctrl_ptr && 0 != basepicker_ctrl_ptr->fling_timer_id)
    {
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        // ��Ϊ�п��ܻ��ж����ͬʱ�����������ʱ��������Ҫ�жϸ������Ƿ����ã�
        // �پ����Ƿ�Ҫͣ��
        for (i = start_field; i < end_field; i++)
        {
            if (MMK_TP_SLIDE_FLING == basepicker_ctrl_ptr->state[i].slide_state && i != field)
            {
                can_stop = FALSE;
                break;
            }
        }

        if (can_stop)
        {
            MMK_StopTimer (basepicker_ctrl_ptr->fling_timer_id);
            basepicker_ctrl_ptr->fling_timer_id = 0;
        }
    }
}

/*****************************************************************************/
//  Description : ����fling״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerHandleFlingTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr     // [in]
)
{
    BOOLEAN is_fling_state = FALSE;
    uint16  item_height = 0;
    int32   i = 0;
    int32   old_pos = 0;        // ԭ��δ�޸�֮ǰ��λ��
    int32   cur_pos = 0;        // �ӹ�ƫ�������λ��
    int32   item_offset = 0;
    int32   start_field = 0;
    int32   end_field = 0;

    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

    for (i = start_field; i < end_field; i++)
    {
        if (MMK_TP_SLIDE_FLING == basepicker_ctrl_ptr->state[i].slide_state)
        {
            BasePickerStopFlingTimer (basepicker_ctrl_ptr, i);

            old_pos = basepicker_ctrl_ptr->state[i].cur_offset;

            // ������ٶ�
            cur_pos = MMK_GetFlingOffset (
                          old_pos, basepicker_ctrl_ptr->state[i].fling_velocity,
                          (float) FLING_TIME, &basepicker_ctrl_ptr->state[i].fling_velocity);

            is_fling_state = FALSE;

            if (FLING_RUN_MIN_VELOCITY <= abs ( (int32) basepicker_ctrl_ptr->state[i].fling_velocity)) // �����Լ����˶�
            {
                // û�ı������£���RECOVER״̬
                if (SetBasePickerOffset (basepicker_ctrl_ptr, i, old_pos - cur_pos))
                {
                    BasePickerStartFlingTimer (basepicker_ctrl_ptr);
                    is_fling_state = TRUE;
                }
            }

            if (!is_fling_state)    // С����С���ٶȣ�����ָ�״̬
            {
                item_height = GetItemHeight (basepicker_ctrl_ptr);
                item_offset = basepicker_ctrl_ptr->state[i].cur_offset % item_height;

                basepicker_ctrl_ptr->state[i].recover_cur_offset = basepicker_ctrl_ptr->state[i].cur_offset;
                basepicker_ctrl_ptr->state[i].recover_dst_offset = basepicker_ctrl_ptr->state[i].cur_offset - item_offset;

                if ( ( (int32) basepicker_ctrl_ptr->state[i].fling_velocity) > 0)   // �����϶����������Ҫ������
                {
                    basepicker_ctrl_ptr->state[i].recover_move_step = -MMITHEME_BASEPICKER_RECOVER_DISTANCE;
                }
                else                                                    // �����϶������
                {
                    basepicker_ctrl_ptr->state[i].recover_move_step = MMITHEME_BASEPICKER_RECOVER_DISTANCE;

                    if (basepicker_ctrl_ptr->state[i].is_loop)
                    {
                        basepicker_ctrl_ptr->state[i].recover_dst_offset += item_height;
                    }
                }

                basepicker_ctrl_ptr->state[i].slide_state = MMK_TP_SLIDE_RECOVER;
                BasePickerStartRecoverTimer (basepicker_ctrl_ptr);
            }
        }
    }
}

/*****************************************************************************/
//  Description : ����recover״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStartRecoverTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr       // [in]
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    if (PNULL != basepicker_ctrl_ptr && 0 == basepicker_ctrl_ptr->recover_timer_id)
    {
        basepicker_ctrl_ptr->recover_timer_id = MMK_CreateWinTimer (
                                                    base_ctrl_ptr->handle,
                                                    MMITHEME_BASEPICKER_RECOVER_TIME,
                                                    TRUE);
    }
}

/*****************************************************************************/
//  Description : �ر�recover״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStopRecoverTimer (
    CTRLBASEPICKER_OBJ_T           *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T         field                   // [in] ָ���ĸ���ģ�����������������ã��Ͳ���ֹͣ
)
{
    BOOLEAN can_stop = TRUE;
    int32   i = 0;
    int32   start_field = 0;
    int32   end_field = 0;

    if (PNULL != basepicker_ctrl_ptr && 0 != basepicker_ctrl_ptr->recover_timer_id)
    {
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        // ��Ϊ�п��ܻ��ж����ͬʱ�����������ʱ��������Ҫ�жϸ������Ƿ����ã�
        // �پ����Ƿ�Ҫͣ��
        for (i = start_field; i < end_field; i++)
        {
            if (MMK_TP_SLIDE_RECOVER == basepicker_ctrl_ptr->state[i].slide_state && i != field)
            {
                can_stop = FALSE;
                break;
            }
        }

        if (can_stop)
        {
            MMK_StopTimer (basepicker_ctrl_ptr->recover_timer_id);
            basepicker_ctrl_ptr->recover_timer_id = 0;
        }
    }
}

/*****************************************************************************/
//  Description : ����ָ�״̬�Ķ�ʱ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerHandleRecoverTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr   // [in]
)
{
    BOOLEAN is_need_check_value = TRUE;     // �Ƿ���Ҫ���ʱ��
    int32   i = 0;
    int32   start_field = 0;
    int32   end_field = 0;
    int32   recover_old_offset = 0;

    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

    for (i = start_field; i < end_field; i++)
    {
        if (MMK_TP_SLIDE_RECOVER == basepicker_ctrl_ptr->state[i].slide_state)
        {
            BasePickerStopRecoverTimer (basepicker_ctrl_ptr, i);

            recover_old_offset = basepicker_ctrl_ptr->state[i].recover_cur_offset;

            basepicker_ctrl_ptr->state[i].recover_cur_offset += basepicker_ctrl_ptr->state[i].recover_move_step;

            // �Ѿ��ָ���Ŀ��λ�ã����ǻ���ͷ��������ı�״̬
            if (0 == basepicker_ctrl_ptr->state[i].recover_move_step     // ̧��ʱ������ԭλ
                    || (basepicker_ctrl_ptr->state[i].recover_cur_offset <= basepicker_ctrl_ptr->state[i].recover_dst_offset    // ����ͷ��
                        && basepicker_ctrl_ptr->state[i].recover_move_step < 0)
                    || (basepicker_ctrl_ptr->state[i].recover_cur_offset >= basepicker_ctrl_ptr->state[i].recover_dst_offset    // �ӹ�ͷ��
                        && basepicker_ctrl_ptr->state[i].recover_move_step > 0))
            {
                // ����scroll״̬������none״̬
                basepicker_ctrl_ptr->state[i].recover_cur_offset = basepicker_ctrl_ptr->state[i].recover_dst_offset;
                basepicker_ctrl_ptr->state[i].slide_state = MMK_TP_SLIDE_NONE;

                // �������֪ͨӦ��
                PostPickerSelectedMsg (basepicker_ctrl_ptr);
            }
            else
            {
                // ����scroll
                BasePickerStartRecoverTimer (basepicker_ctrl_ptr);
            }

            SetBasePickerOffset (basepicker_ctrl_ptr, i, basepicker_ctrl_ptr->state[i].recover_cur_offset - recover_old_offset);
        }

        // �������������scroll, �ǲ���Ҫ���ֵ
        if (MMK_TP_SLIDE_NONE != basepicker_ctrl_ptr->state[i].slide_state)
        {
            is_need_check_value = FALSE;
        }
    }

    // ����ǰ��µ������Ҳ����Ҫ���
    if (is_need_check_value && BASEPICKER_FIELD_NONE == basepicker_ctrl_ptr->tp_down_field)
    {
        VTLBASEPICKER_CheckValue (basepicker_ctrl_ptr, TRUE);
    }
}

/*****************************************************************************/
//  Description : ����ĳ���������
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayField (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T        field,                  // [in]
    GUI_RECT_T                    *dis_rect_ptr           // [in] ��ʾ����
)
{
    BOOLEAN     result = FALSE;
    uint16      client_width = 0;
    uint16      client_height = 0;
    GUI_RECT_T  img_rect = {0};                 // ͼƬ����ʾ����(��������)
    GUI_RECT_T  relative_dis_rect = {0};        // ��ʾ������������(�����ͼƬ�������)
    GUI_RECT_T  text_rect = {0};                // �����ж��ı��Ƿ��ڿ�������
    GUI_POINT_T start_point = {0};

    // ��ȡ��ʾ�����λ�ã���ͼƬ�Ŀ����Ϣ
    result = GetStartPoint (basepicker_ctrl_ptr, &start_point);

    if (!result)
    {
        return;
    }

    GetBasePickerSize (basepicker_ctrl_ptr, &client_width, &client_height);

    // ͼƬ������(ת��Ϊ����λ��)
    img_rect.left = start_point.x;
    img_rect.right = img_rect.left + client_width - 1;
    img_rect.top = start_point.y;
    img_rect.bottom = img_rect.top + client_height - 1;

    // �ܹ���ʾ�����
    if (GUI_IntersectRect (&img_rect, img_rect, *dis_rect_ptr))
    {
        // ��Ҫ���Ƶ��������
        relative_dis_rect = img_rect;
        TranslateRect (&relative_dis_rect, -start_point.x, -start_point.y);

        // ������ͼ
        BasePickerDisplayBg (basepicker_ctrl_ptr, &img_rect, &relative_dis_rect);

        // ���ɲ�
        // picker����ɰ�������ֵ���
        if (!basepicker_ctrl_ptr->theme.is_mask_on_top)
        {
            BasePickerDisplayMask (basepicker_ctrl_ptr, dis_rect_ptr);
        }

        // ������
        if (GUI_IntersectRect (&text_rect, VTLBASEPICKER_GetFieldRect (basepicker_ctrl_ptr, field), relative_dis_rect))
        {
            BasePickerDisplayText (basepicker_ctrl_ptr, field, &img_rect);
        }

        // ���ɲ�
        // ��picker����ɰ�����������
        if (basepicker_ctrl_ptr->theme.is_mask_on_top)
        {
            BasePickerDisplayMask (basepicker_ctrl_ptr, dis_rect_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : ���Ʊ���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayBg (
    CTRLBASEPICKER_OBJ_T      *basepicker_ctrl_ptr,     // [in]
    GUI_RECT_T                *dis_rect_ptr,
    GUI_RECT_T                *img_rect_ptr
)
{
    GUI_RECT_T          img_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    //set image rect
    img_rect.left   = (int16) (dis_rect_ptr->left - base_ctrl_ptr->rect.left);
    img_rect.top    = (int16) (dis_rect_ptr->top - base_ctrl_ptr->rect.top);
    img_rect.right  = (int16) (img_rect.left + dis_rect_ptr->right - dis_rect_ptr->left);
    img_rect.bottom = (int16) (img_rect.top + dis_rect_ptr->bottom - dis_rect_ptr->top);

    //set bg display info
    bg_display.is_transparent = FALSE;
    bg_display.rect           = base_ctrl_ptr->rect;
    bg_display.display_rect   = *dis_rect_ptr;
    bg_display.img_rect       = img_rect;
    bg_display.ctrl_handle    = base_ctrl_ptr->handle;
    bg_display.win_handle     = base_ctrl_ptr->win_handle;

    //display bg
    GUI_DisplayBg (&base_ctrl_ptr->bg,
                   &bg_display,
                   &base_ctrl_ptr->lcd_dev_info);
}

/*****************************************************************************/
//  Description : ���Ʊ߿�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayBorder (
    CTRLBASEPICKER_OBJ_T  *basepicker_ctrl_ptr,   // [in]
    GUI_RECT_T            *client_rect_ptr,       // [in]
    GUI_RECT_T            *img_rect_ptr           // [in]
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    GUIRES_DisplayImg (
        PNULL,
        client_rect_ptr,
        img_rect_ptr,
        base_ctrl_ptr->win_handle,
        basepicker_ctrl_ptr->theme.border_image_id,
        (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
}

/*****************************************************************************/
//  Description : ��ʾ�ı�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayText (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    GUI_RECT_T                  *disp_rect_ptr          // [in]
)
{
    uint16          string_width = 0;
    int32           i = 0;
    GUI_POINT_T     start_point = {0};
    GUI_RECT_T      field_rect = {0};
    GUI_RECT_T      tips_rect = {0};
    GUISTR_STYLE_T  str_style = {0};
    GUISTR_STATE_T  str_state = 0;
    BASEPICKER_FIELD_INFO_T  field_info[BASEPICKER_MAX_DISP_COUNT] = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    // ��ȡ��ʾ��Ϣ
    GetBasePickerInfo (basepicker_ctrl_ptr, field, field_info, BASEPICKER_MAX_DISP_COUNT);

    //set string style
    str_style.align      = ALIGN_HVMIDDLE;
    str_style.char_space = basepicker_ctrl_ptr->theme.char_space;

    str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;

    for (i = 0; i < BASEPICKER_MAX_DISP_COUNT; i++)
    {
        // ��ʾ���ַ����������������ÿ�ζ���ʼ��
        wchar           display_str[CTRLBASEPICKER_MAX_STR_LEN] = {0};
        MMI_STRING_T    dis_string = {0};

        dis_string.wstr_ptr = (wchar*) display_str;

        VTLBASEPICKER_GetFieldText (basepicker_ctrl_ptr, field, field_info[i].value, &dis_string);

        str_style.font_color = field_info[i].font.color;
        str_style.font = field_info[i].font.font;

        field_rect = field_info[i].disp_rect;

        if (CTRLBASEPICKER_TIPS_POS_MIDDLE == basepicker_ctrl_ptr->tips_pos)
        {
            field_rect.left -= BASEPICKER_TIPS_OFFSET;
            field_rect.right -= BASEPICKER_TIPS_OFFSET;
            //field_rect.right = (field_rect.right + field_rect.left) / 2 - BASEPICKER_TIPS_SPACE;
        }

        GUISTR_DrawTextToLCDInRect (
            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
            (const GUI_RECT_T*) &field_rect,
            (const GUI_RECT_T*) disp_rect_ptr,
            &dis_string,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO);

        if (0 == string_width
                && CTRLBASEPICKER_TIPS_POS_MIDDLE == basepicker_ctrl_ptr->tips_pos)
        {
            string_width = GUI_GetStringWidthEx (
                               basepicker_ctrl_ptr->theme.cur_content_font.font,
                               dis_string.wstr_ptr, dis_string.wstr_len,
                               str_style.char_space);
        }
    }

    // middle tips
    if (CTRLBASEPICKER_TIPS_POS_MIDDLE == basepicker_ctrl_ptr->tips_pos)
    {
        //set rect
        GetStartPoint (basepicker_ctrl_ptr, &start_point);

        tips_rect = VTLBASEPICKER_GetFieldRect (basepicker_ctrl_ptr, field);
        TranslateRect (&tips_rect, start_point.x, start_point.y);

        tips_rect.left = (tips_rect.left + tips_rect.right + string_width) / 2 + BASEPICKER_TIPS_SPACE - BASEPICKER_TIPS_OFFSET;

        BasePickerDisplayTips (basepicker_ctrl_ptr, field, &tips_rect, disp_rect_ptr);
    }
}

/*****************************************************************************/
//  Description : ���Ʒָ���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplaySeparator (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    GUI_RECT_T             *separator_rect_ptr,    // [in]
    GUI_RECT_T             *relative_rect_ptr,     // [in]
    GUI_POINT_T            *start_pt_ptr,          // [in]
    MMI_IMAGE_ID_T         separator_image_id      // [in]
)
{
    GUI_RECT_T      img_rect = {0};         // ��ʾ��λ��
    GUI_RECT_T      sep_img_rect = {0};     // ͼƬ�е�λ��
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    if (GUI_IntersectRect (&img_rect, *separator_rect_ptr, *relative_rect_ptr))
    {
        sep_img_rect = img_rect;

        TranslateRect (&img_rect, start_pt_ptr->x, start_pt_ptr->y);
        TranslateRect (&sep_img_rect, -separator_rect_ptr->left, -separator_rect_ptr->top);

        GUIRES_DisplayImg (
            PNULL,
            &img_rect,
            &sep_img_rect,
            base_ctrl_ptr->win_handle,
            separator_image_id,
            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
    }
}

/*****************************************************************************/
//  Description : �����ɰ�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayMask (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,     // [in]
    GUI_RECT_T              *dis_rect_ptr
)
{
    BOOLEAN     result = FALSE;
    uint16      image_height = 0;
    GUI_RECT_T  mask_rect = {0};
    GUI_RECT_T  dst_mask_rect = {0};
    GUI_RECT_T  relative_dis_rect = {0};
    GUI_POINT_T start_point = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    // ��ȡ��ʾ�����λ�ã���ͼƬ�Ŀ����Ϣ
    result = GetStartPoint (basepicker_ctrl_ptr, &start_point);

    if (!result)
    {
        return;
    }

    mask_rect = BASEPICKER_GetTextRect (basepicker_ctrl_ptr);

    // PICKER�����item�ĸ߶ȣ�������ͼƬ
    if (GUI_BG_IMG == basepicker_ctrl_ptr->theme.mask_bg.bg_type
            && MMITHEME_CheckImageID (basepicker_ctrl_ptr->theme.mask_bg.img_id))
    {
        GUIRES_GetImgWidthHeight (
            PNULL, &image_height,
            basepicker_ctrl_ptr->theme.mask_bg.img_id,
            base_ctrl_ptr->win_handle);
    }
    else
    {
        image_height = GetItemHeight (basepicker_ctrl_ptr);
    }

    mask_rect.top = (mask_rect.top + mask_rect.bottom - image_height + 1) / 2;
    mask_rect.bottom = mask_rect.top + image_height - 1;

    // ͼƬ������(ת��Ϊ����λ��)
    mask_rect.left += start_point.x;
    mask_rect.right += start_point.x;
    mask_rect.top += start_point.y;
    mask_rect.bottom += start_point.y;

    dst_mask_rect = mask_rect;

    // �ܹ���ʾ�����
    if (GUI_IntersectRect (&dst_mask_rect, dst_mask_rect, *dis_rect_ptr))
    {
        if (GUI_BG_IMG == basepicker_ctrl_ptr->theme.mask_bg.bg_type
                && MMITHEME_CheckImageID (basepicker_ctrl_ptr->theme.mask_bg.img_id))
        {
            relative_dis_rect.left = dst_mask_rect.left - mask_rect.left;
            relative_dis_rect.right = dst_mask_rect.right - mask_rect.left;
            relative_dis_rect.top = dst_mask_rect.top - mask_rect.top;
            relative_dis_rect.bottom = dst_mask_rect.bottom - mask_rect.top;

            // ��������ɲ�
            GUIRES_DisplayImg (
                PNULL,
                &dst_mask_rect,
                &relative_dis_rect,
                base_ctrl_ptr->win_handle,
                basepicker_ctrl_ptr->theme.mask_bg.img_id,
                (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
        }
        else
        {
            GUI_FillRect (
                (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
                dst_mask_rect,
                basepicker_ctrl_ptr->theme.mask_bg.color);
        }
    }
}

/*****************************************************************************/
//  Description : ������ʾ�ı�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayTips (
    CTRLBASEPICKER_OBJ_T            *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T          field,                  // [in]
    GUI_RECT_T                      *dis_rect_ptr,          // [in]
    GUI_RECT_T                      *clip_rect_ptr          // [in]
)
{
    GUISTR_STYLE_T  str_style = {0};
    GUISTR_STATE_T  str_state = 0;
    MMI_TEXT_ID_T   tips_text_id = {0};
    MMI_STRING_T    tips_string = {0};
    GUI_RECT_T      img_rect = {0};
    uint16          img_height = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    // ��ʾ���ֱ���
    if (CTRLBASEPICKER_TIPS_POS_TOP == basepicker_ctrl_ptr->tips_pos)
    {
        img_rect.right = clip_rect_ptr->right - clip_rect_ptr->left;
        img_rect.bottom = clip_rect_ptr->bottom - clip_rect_ptr->top;

        GUIRES_DisplayImg (
            PNULL,
            clip_rect_ptr,
            &img_rect,
            base_ctrl_ptr->win_handle,
            basepicker_ctrl_ptr->theme.tips_image_id,
            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
    }

    tips_text_id = VTLBASEPICKER_GetFieldTips (basepicker_ctrl_ptr, field);

    if (0 != tips_text_id)
    {
        if (CTRLBASEPICKER_TIPS_POS_TOP == basepicker_ctrl_ptr->tips_pos)
        {
            // ���ݱ����ĸ߶�
            GUIRES_GetImgWidthHeight (
                PNULL, &img_height,
                basepicker_ctrl_ptr->theme.tips_image_id,
                base_ctrl_ptr->win_handle);

            dis_rect_ptr->bottom = dis_rect_ptr->top + img_height - 1;

            str_style.align      = ALIGN_HVMIDDLE;
            str_style.font_color = basepicker_ctrl_ptr->theme.tips_font.color;
            str_style.font       = basepicker_ctrl_ptr->theme.tips_font.font;
        }
        else
        {
            str_style.align      = ALIGN_LVMIDDLE;
            str_style.font_color = basepicker_ctrl_ptr->theme.tips_font_middle.color;
            str_style.font       = basepicker_ctrl_ptr->theme.tips_font_middle.font;
        }

        str_style.char_space = basepicker_ctrl_ptr->theme.char_space;

        str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR;

        MMITHEME_GetLabelTextByLang (tips_text_id, &tips_string);

        GUISTR_DrawTextToLCDInRect (
            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
            (const GUI_RECT_T*) dis_rect_ptr,
            (const GUI_RECT_T*) clip_rect_ptr,
            &tips_string,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO);
    }
}

/*****************************************************************************/
//  Description : ����ƫ��λ�ã����Զ����½���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetBasePickerOffset (
    CTRLBASEPICKER_OBJ_T           *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T         field,                  // [in]
    int32                          offset                  // [in] �뵱ǰֵ��ƫ����
)
{
    BOOLEAN         is_handle = TRUE;
    uint16          min_value = 0;
    uint16          max_value = 0;
    uint16          item_height = 0;
    int32           max_offset = 0;
    int32           old_offset = 0;
    GUI_POINT_T     start_point = {0};
    GUI_RECT_T      cross_rect = {0};
    GUI_RECT_T      field_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    if (0 == offset)
    {
        return FALSE;
    }

    item_height = GetItemHeight (basepicker_ctrl_ptr);

    VTLBASEPICKER_GetValueRange (basepicker_ctrl_ptr, field, &min_value, &max_value);

    // ���ĳ���
    max_offset = (max_value - min_value + 1) * item_height;

    // ����ƫ����
    old_offset = basepicker_ctrl_ptr->state[field].cur_offset;
    basepicker_ctrl_ptr->state[field].cur_offset += offset;

    // ��ѭ���뵥���������
    //  ѭ��ʱ��������ֵ����Ϊ��һ��
    //  ����ʱ��������ֵ����Ϊ��ֵ
    if (basepicker_ctrl_ptr->state[field].is_loop)
    {
        if (basepicker_ctrl_ptr->state[field].cur_offset < 0)
        {
            basepicker_ctrl_ptr->state[field].cur_offset += max_offset;
        }
        else if (basepicker_ctrl_ptr->state[field].cur_offset >= max_offset)
        {
            basepicker_ctrl_ptr->state[field].cur_offset -= max_offset;
        }

        if (basepicker_ctrl_ptr->state[field].cur_offset < 0
                || basepicker_ctrl_ptr->state[field].cur_offset >= max_offset)
        {
            basepicker_ctrl_ptr->state[field].cur_offset = 0;
        }
    }
    else
    {
        if (basepicker_ctrl_ptr->state[field].cur_offset < 0)
        {
            basepicker_ctrl_ptr->state[field].cur_offset = 0;
            is_handle = FALSE;
        }
        else if (basepicker_ctrl_ptr->state[field].cur_offset >= max_offset - item_height)
        {
            basepicker_ctrl_ptr->state[field].cur_offset = max_offset - item_height;
            is_handle = FALSE;
        }
    }

    // ���ֵ����Ч��
    CheckValid (basepicker_ctrl_ptr, field);

    // ����
    if (old_offset != basepicker_ctrl_ptr->state[field].cur_offset
            && GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, base_ctrl_ptr->rect))
    {
        GetStartPoint (basepicker_ctrl_ptr, &start_point);

        field_rect = VTLBASEPICKER_GetFieldRect (basepicker_ctrl_ptr, field);
        TranslateRect (&field_rect, start_point.x, start_point.y);

        if (GUI_IntersectRect (&field_rect, cross_rect, field_rect))
        {
            BasePickerDisplayField (basepicker_ctrl_ptr, field, &field_rect);
        }
    }

    return is_handle;
}

/*****************************************************************************/
//  Description : ����������ֵ�Ƿ�Ϸ�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CheckValid (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T        field                   // [in]
)
{
    uint16  cur_value = 0;
    uint16  min_value = 0;
    uint16  max_value = 0;

    VTLBASEPICKER_GetValueRange (basepicker_ctrl_ptr, field, &min_value, &max_value);

    cur_value = BASEPICKER_GetValue (basepicker_ctrl_ptr, field);

    SCI_ASSERT (cur_value >= min_value && cur_value <= max_value);      /*assert verified*/
}

/*****************************************************************************/
//  Description : ���ݶ��뷽ʽ����ȡ��ʾ�����λ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetStartPoint (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    GUI_POINT_T             *start_point_ptr        // [out]
)
{
    uint16          client_width = 0;
    uint16          client_height = 0;
    GUI_ALIGN_E     align = ALIGN_HVMIDDLE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != start_point_ptr);       /*assert verified*/
    if (PNULL == start_point_ptr)
    {
        return FALSE;
    }

    GetBasePickerSize (basepicker_ctrl_ptr, &client_width, &client_height);

    align = GetAlign (basepicker_ctrl_ptr);

    if (ALIGN_LVMIDDLE == align)
    {
        start_point_ptr->x = base_ctrl_ptr->rect.left;
    }
    else if (ALIGN_RVMIDDLE == align)
    {
        start_point_ptr->x = base_ctrl_ptr->rect.right - (int16) client_width;
    }
    else
    {
        start_point_ptr->x = (base_ctrl_ptr->rect.left + base_ctrl_ptr->rect.right + 1 - (int16) client_width) / 2;
    }

    start_point_ptr->y = (base_ctrl_ptr->rect.top + base_ctrl_ptr->rect.bottom + 1 - (int16) client_height) / 2;

    return TRUE;
}

/*****************************************************************************/
//  Description : ���ÿ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void SetBasePickerSize (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,         // [in]
    uint16                 width,                  // [in]
    uint16                 height                  // [in]
)
{
    if (0 != width)
    {
        basepicker_ctrl_ptr->theme.client_width = width;
    }

    if (0 != height)
    {
        basepicker_ctrl_ptr->theme.client_height = height;
    }
}

/*****************************************************************************/
//  Description : ��ȡ���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GetBasePickerSize (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    uint16                 *width_ptr,             // [out]
    uint16                 *height_ptr             // [out]
)
{
    if (PNULL != width_ptr)
    {
        *width_ptr = basepicker_ctrl_ptr->theme.client_width;
    }

    // �߶�Ϊ��������ͼ�ĸ߶�
    if (PNULL != height_ptr)
    {
        *height_ptr = basepicker_ctrl_ptr->theme.client_height;
    }
}

/*****************************************************************************/
//  Description : ��ȡ�о�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TranslateRect (
    GUI_RECT_T     *rect_ptr,      // [in/out]
    int16          x,              // [in]
    int16          y               // [in]
)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != rect_ptr);      /*assert verified*/
    if (PNULL != rect_ptr)
    {
        rect_ptr->left += x;
        rect_ptr->right += x;
        rect_ptr->top += y;
        rect_ptr->bottom += y;
    }
}

/*****************************************************************************/
//  Description : ��ȡλ�ú�������Ϣ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetBasePickerInfo (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,     // [in]
    CTRLBASEPICKER_FIELD_T      field,              // [in]
    BASEPICKER_FIELD_INFO_T     *field_info_ptr,    // [out] ���ص������Ǿ�������
    int32                       field_info_count    // [in]
)
{
    uint16      item_height = 0;        // ÿ��item�߶�
    uint16      date_value = 0;         // ��ǰ���ڼ����item��ֵ
    uint16      cur_date_value = 0;     // �м�item��ֵ
    int32       i = 0;
    int32       start_pos = 0;          // �м�item����ʼλ��
    int32       cur_pos = 0;            // ��ǰ���ڼ����item��λ��
    int32       middle_pos = 0;         // �м�λ��
    int32       cur_index = 0;          // ��field_info_ptr��ƫ��
    GUI_POINT_T start_point = {0};
    GUI_RECT_T  field_rect = {0};       // ��ʾ����

    //set rect
    GetStartPoint (basepicker_ctrl_ptr, &start_point);

    field_rect = VTLBASEPICKER_GetFieldRect (basepicker_ctrl_ptr, field);
    TranslateRect (&field_rect, start_point.x, start_point.y);

    // get current date value
    cur_date_value = BASEPICKER_GetValue (basepicker_ctrl_ptr, field);
    date_value = cur_date_value;

    // height of a item
    item_height = GetItemHeight (basepicker_ctrl_ptr);

    // start position of the current item
    middle_pos = (field_rect.bottom + field_rect.top + 1) / 2;
    start_pos = (field_rect.bottom + field_rect.top + 1 - item_height) / 2 + 1 - basepicker_ctrl_ptr->state[field].cur_offset % item_height;
    cur_pos = start_pos;

    //SCI_TRACE_LOW:"== GetBasePickerInfo == y: %d, value: %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TOUCH_1820_112_2_18_3_16_25_142, (uint8*) "dd", start_pos, cur_date_value);

    // �ҵ�ǰֵ�����
    for (i = 0; i < field_info_count; i++)
    {
        if (cur_pos > field_rect.bottom)
        {
            break;
        }
        else
        {
            // ��ֵ����ʾ����
            field_info_ptr[cur_index].disp_rect = field_rect;
            field_info_ptr[cur_index].disp_rect.top = cur_pos;
            field_info_ptr[cur_index].disp_rect.bottom = cur_pos + item_height - 1;

            // ֵ
            field_info_ptr[cur_index].value = date_value;

            // ��ȡ��ɫ��Ϣ
            if (field_info_ptr[cur_index].disp_rect.top <= middle_pos
                    && field_info_ptr[cur_index].disp_rect.bottom > middle_pos)
            {
                field_info_ptr[cur_index].font = basepicker_ctrl_ptr->theme.cur_content_font;
            }
            else
            {
                field_info_ptr[cur_index].font = basepicker_ctrl_ptr->theme.content_font;
            }

            cur_index++;
            cur_pos += item_height;

            if (!GetNextValue (basepicker_ctrl_ptr, field, date_value, TRUE, &date_value))
            {
                break;
            }
        }
    }

    // �ҵ�ǰֵ�����
    if (GetNextValue (basepicker_ctrl_ptr, field, cur_date_value, FALSE, &date_value))
    {
        cur_pos = start_pos - 1;

        for (i = cur_index; i < field_info_count; i++)
        {
            if (cur_pos < field_rect.top)
            {
                break;
            }
            else
            {
                field_info_ptr[cur_index].disp_rect = field_rect;
                field_info_ptr[cur_index].disp_rect.bottom = cur_pos;
                field_info_ptr[cur_index].disp_rect.top = cur_pos - item_height + 1;

                field_info_ptr[cur_index].value = date_value;

                // ��ȡ��ɫ��Ϣ
                if (field_info_ptr[cur_index].disp_rect.top <= middle_pos
                        && field_info_ptr[cur_index].disp_rect.bottom > middle_pos)
                {
                    field_info_ptr[cur_index].font = basepicker_ctrl_ptr->theme.cur_content_font;
                }
                else
                {
                    field_info_ptr[cur_index].font = basepicker_ctrl_ptr->theme.content_font;
                }

                cur_index++;
                cur_pos -= item_height;

                if (!GetNextValue (basepicker_ctrl_ptr, field, date_value, FALSE, &date_value))
                {
                    break;
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : ���ݵ�ǰֵ����ȡ��һ��������һ��ֵ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//  �����Ƿ�ɹ�������ǵ���ģ���ͷ֮�󷵻�FALSE
/*****************************************************************************/
LOCAL BOOLEAN GetNextValue (
    CTRLBASEPICKER_OBJ_T         *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T       field,                  // [in]
    uint16                       cur_date_value,         // [in]
    BOOLEAN                      is_next,                // [in]
    uint16                       *next_value_ptr         // [out]
)
{
    BOOLEAN is_succeeded = TRUE;
    uint16 min_value = 0;
    uint16 max_value = 0;

    VTLBASEPICKER_GetValueRange (basepicker_ctrl_ptr, field, &min_value, &max_value);

    if (is_next)
    {
        if (cur_date_value >= max_value)
        {
            if (basepicker_ctrl_ptr->state[field].is_loop)
            {
                *next_value_ptr = min_value;
            }
            else
            {
                is_succeeded = FALSE;
            }
        }
        else
        {
            *next_value_ptr = cur_date_value + 1;
        }
    }
    else
    {
        if (cur_date_value <= min_value)
        {
            if (basepicker_ctrl_ptr->state[field].is_loop)
            {
                *next_value_ptr = max_value;
            }
            else
            {
                is_succeeded = FALSE;
            }
        }
        else
        {
            *next_value_ptr = cur_date_value - 1;
        }
    }

    return is_succeeded;
}

/*****************************************************************************/
//  Description : ����ѡ����Ϣ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL void PostPickerSelectedMsg (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr     // [in]
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    // picker������Ѿ�ֹͣ������·���Ϣ
    if (0 == basepicker_ctrl_ptr->recover_timer_id
            && 0 == basepicker_ctrl_ptr->fling_timer_id
            && 0 == basepicker_ctrl_ptr->tpscroll_timer_id)
    {
        CTRLMSG_PostNotify (base_ctrl_ptr->handle, MSG_NOTIFY_PICKER_SELECTED);
    }
}

/*****************************************************************************/
//  Description : ��ȡitem�ĸ߶�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL uint16 GetItemHeight (
    CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr  // [in]
)
{
    return basepicker_ctrl_ptr->theme.line_height;
}

/*****************************************************************************/
//  Description : get align
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
__inline LOCAL GUI_ALIGN_E GetAlign (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr
)
{
    return basepicker_ctrl_ptr->theme.align;
}

/*****************************************************************************/
//  Description : ����DOWNʱ�ĵ㣬�ж��Ƿ��ƶ���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL BOOLEAN IsTpMoved (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    GUI_POINT_T            *point_ptr              // [in]
)
{
    if (abs (point_ptr->x - basepicker_ctrl_ptr->tp_start_point.x) > BASEPICKER_TP_MOVE_DIS
            || abs (point_ptr->y - basepicker_ctrl_ptr->tp_start_point.y) > BASEPICKER_TP_MOVE_DIS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : get basepicker pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLBASEPICKER_OBJ_T* GetBasePickerPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (BasePickerTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLBASEPICKER_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN BasePickerTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_BASEPICKER_TYPE);
}

#endif

