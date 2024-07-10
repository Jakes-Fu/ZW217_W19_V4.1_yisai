/*****************************************************************************
** File Name:      ctrltips.c                                                                                                      *
** Author:                                                                                                  *
** Date:                                                                                         *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.                                 *
** Description:    This file is used to describe call log                                                           *
******************************************************************************
**                         Important Edit History                                                                          *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                     *
** 7/2012        xiyuan.ma        Create                                                                              *
*********************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmi_theme.h"
#include "mmitheme_tips.h"
#include "guires.h"
#include "guistring.h"
#include "ui_layer.h"

#include "ctrltips.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
// tips显示位置距离tips区域水平或垂直方向上的距离
#define		HORIZONTAL_GAP      4
#define		VERTICAL_GAP        2

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
/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init tips class
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TipsInitVtbl (
    TIPS_VTBL_T *tips_vtbl_ptr
);

/*****************************************************************************/
//  Description : tips construct
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TipsConstruct (
    CTRLBASE_OBJ_T *obj_ptr,
    void *tips_param_ptr
);

/*****************************************************************************/
//  Description : tips destruct
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TipsDestruct (
    CTRLBASE_OBJ_T *obj_ptr
);

/*****************************************************************************/
// Description : handle tips message
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TipsHandleMessage (
    CTRLBASE_OBJ_T        *obj_ptr, //控件的指针
    MMI_MESSAGE_ID_E  msg_id,   //事件类型
    DPARAM            param     //附带的处理参数
);

/*****************************************************************************/
//  Description : calculate tips' rect
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void TipsCalculateRect (
    TIPS_OBJ_T *tips_ptr,
    GUI_RECT_T	*orig_rect_ptr
);

/*****************************************************************************/
//  Description : draw tips
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void TipsDrawBox (
    TIPS_OBJ_T *tips_ptr
);

/*****************************************************************************/
//  Description : draw tips background
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void TipsDrawBackgroud (
    TIPS_OBJ_T *tips_ptr
);

/*****************************************************************************/
//  Description : statusbar type of
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:该函数进行运行时类型判别
/*****************************************************************************/
LOCAL BOOLEAN TipsTypeOf (
    CTRLBASE_OBJ_T* ctrl_ptr
);

/*****************************************************************************/
//  Description : draw tips background
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void TipsDrawText (
    TIPS_OBJ_T *tips_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void TipsCtrlPackInitParam (
    GUITIPS_INIT_DATA_T     *tips_init_ptr,   //in
    TIPS_INIT_PARAM_T       *tips_param_ptr   //out
);
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                                                                        *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get tips type
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T CTRLTIPS_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,
                       "tips",
                       sizeof (TIPS_OBJ_T),
                       (OBJECT_CONSTRUCTOR_FUNC) TipsConstruct,
                       (OBJECT_DESTRUCTOR_FUNC) TipsDestruct,
                       sizeof (TIPS_INIT_PARAM_T),
                       (PARAM_PACKING_FUNC)TipsCtrlPackInitParam,
                       sizeof (TIPS_VTBL_T),
                       (VTBL_INIT_FUNC) TipsInitVtbl);
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init tips class
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TipsInitVtbl (
    TIPS_VTBL_T *tips_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) tips_vtbl_ptr;

    base_ctrl_vtbl_ptr->HandleEvent = TipsHandleMessage;
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void TipsCtrlPackInitParam (
    GUITIPS_INIT_DATA_T     *tips_init_ptr,   //in
    TIPS_INIT_PARAM_T       *tips_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*)tips_param_ptr;

    if (PNULL == tips_init_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = tips_init_ptr->both_rect;
}

/*****************************************************************************/
//  Description : construct
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TipsConstruct (
    CTRLBASE_OBJ_T *obj_ptr,
    void *init_data_ptr
)
{
    TIPS_OBJ_T *tips_ptr = PNULL;
    TIPS_INIT_PARAM_T *init_ptr = PNULL;

    if (PNULL == obj_ptr || PNULL == init_data_ptr)
    {
        return FALSE;
    }

    tips_ptr = (TIPS_OBJ_T *) obj_ptr;
    init_ptr = (TIPS_INIT_PARAM_T *) init_data_ptr;

    MMITHEME_GetTipsStyle (&tips_ptr->style_info);
    tips_ptr->base_ctrl.lcd_dev_info = * (MMITHEME_GetDefaultLcdDev());

    // 默认情况下,创建tips的时候,有效性为True
    tips_ptr->is_valid = TRUE;

    //set state
    VTLBASE_SetCanActive(obj_ptr, FALSE);

    return TRUE;
}

/*****************************************************************************/
//  Description : destruct
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TipsDestruct (
    CTRLBASE_OBJ_T *obj_ptr
)
{
    return TRUE;
}

/*****************************************************************************/
// Description : handle tips message
// Global resource dependence :
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TipsHandleMessage (
    CTRLBASE_OBJ_T *obj_ptr, //控件的指针
    MMI_MESSAGE_ID_E msg_id, //事件类型
    DPARAM param //附带的处理参数
)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    TIPS_OBJ_T *tips_ptr = PNULL;

    if (PNULL == obj_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    tips_ptr = (TIPS_OBJ_T*) obj_ptr;

    switch (msg_id)
    {
    case MSG_CTL_OPEN:
        break;

    case MSG_CTL_LOSE_FOCUS: //失去焦点
        break;

    case MSG_CTL_GET_FOCUS:  //得到焦点
        break;

    case MSG_TIMER:
        break;

    case MSG_CTL_PAINT:
        break;

    default:
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : Tips Type Of
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:该函数进行运行时类型判别
/*****************************************************************************/
LOCAL BOOLEAN TipsTypeOf (
    CTRLBASE_OBJ_T* ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf(ctrl_ptr, CTRL_TIPS_TYPE);
}

/*****************************************************************************/
//  Description : draw tips background
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void TipsDrawBackgroud (
    TIPS_OBJ_T *tips_ptr
)
{
    if (PNULL == tips_ptr)
    {
        return;
    }

    if (tips_ptr->style_info.bg_style.bg_image_info.is_display)
    {
        // 显示背景
    }
    else if (tips_ptr->style_info.bg_style.bg_color_info.is_display)
    {
        // 画背景颜色
        LCD_FillShadowRoundedRect (&tips_ptr->base_ctrl.lcd_dev_info,
                                   tips_ptr->base_ctrl.rect,
                                   tips_ptr->base_ctrl.rect,
                                   tips_ptr->style_info.bg_style.bg_color_info.bg_color);
    }

    // 显示一个tail图片,暂时不支持
}

/*****************************************************************************/
//  Description : draw tips background
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void TipsDrawText (
    TIPS_OBJ_T *tips_ptr
)
{
    GUISTR_STYLE_T text_style = {0}; /*lint !e64*/
    MMI_STRING_T  str_info = {0};

    if (PNULL == tips_ptr)
    {
        return;
    }

    if (0 == tips_ptr->text_info.wstr_len)
    {
        return;
    }

    text_style.align = tips_ptr->style_info.text_style.align;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.font = tips_ptr->style_info.text_style.font_type;
    text_style.font_color = tips_ptr->style_info.text_style.font_color;
    text_style.line_space = 0;

    str_info.wstr_len = tips_ptr->text_info.wstr_len;
    str_info.wstr_ptr = tips_ptr->text_info.wstr_ptr;
    // display string
    GUISTR_DrawTextToLCDInRect (&tips_ptr->base_ctrl.lcd_dev_info,
                                &tips_ptr->text_rect,
                                &tips_ptr->text_rect,
                                &str_info,
                                &text_style,
                                tips_ptr->style_info.text_style.str_state,
                                GUISTR_TEXT_DIR_AUTO
                               );

    return;
}

/*****************************************************************************/
//  Description : draw tips
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void TipsDrawBox (
    TIPS_OBJ_T *tips_ptr
)
{
    TipsDrawBackgroud (tips_ptr);
    TipsDrawText (tips_ptr);
}

/*****************************************************************************/
//  Description : calculate tips' rect
//  Global resource dependence :
//  Author:  xiyuan.ma
//  Note:
/*****************************************************************************/
LOCAL void TipsCalculateRect (
    TIPS_OBJ_T *tips_ptr,
    GUI_RECT_T	    *orig_rect_ptr
)
{
    uint16 tips_width = 0;
    uint16 tips_height = 0;
    GUI_POINT_T start_point = {0};

    if (PNULL == tips_ptr)
    {
        return;
    }

    // 计算Tips的宽度
    tips_width = GUI_GetStringWidth (tips_ptr->style_info.text_style.font_type,
                                     tips_ptr->text_info.wstr_ptr,
                                     tips_ptr->text_info.wstr_len); // 字符串宽度

    // 两端都要加一个间隔
    tips_width += HORIZONTAL_GAP * 2;

    tips_width = MIN (tips_width, tips_ptr->style_info.max_width);

    // 计算Tips的高度
    tips_height = GUI_GetStringHeight (tips_ptr->style_info.text_style.font_type,
                                       tips_ptr->text_info.wstr_ptr,
                                       tips_ptr->text_info.wstr_len);
    // icon 高度
    GUIRES_GetImgWidthHeight (PNULL,
                              &tips_height,
                              tips_ptr->style_info.bg_style.tail1_id,
                              MMK_GetWinHandleByCtrl (tips_ptr->base_ctrl.handle));

    tips_height = MAX (tips_height, tips_ptr->style_info.min_height);

    // 计算
    switch (tips_ptr->style_info.tips_align)
    {
    case MMITIPS_ALIGN_LEFT: // 以原始区域的左侧为基准
        start_point.x = orig_rect_ptr->left;
        break;

    case MMITIPS_ALIGN_RIGHT: // 目前09A版本, 以原始区域的右侧为基准
        start_point.x = orig_rect_ptr->right - tips_width;
        break;

    case MMITIPS_ALIGN_MIDDLE: // 以原始区域的中间为基准
        start_point.x = (orig_rect_ptr->right - orig_rect_ptr->left) / 2;
        break;

    default:
        SCI_ASSERT (0);/*assert verified*/
        break;
    }

    if ( (orig_rect_ptr->bottom + tips_height + VERTICAL_GAP) > tips_ptr->style_info.edge_rect.bottom)
    {
        start_point.y = orig_rect_ptr->top - tips_height - VERTICAL_GAP;
    }
    else
    {
        start_point.y = orig_rect_ptr->bottom + VERTICAL_GAP;
    }

    tips_ptr->base_ctrl.rect.left = start_point.x;
    tips_ptr->base_ctrl.rect.top = start_point.y;
    tips_ptr->base_ctrl.rect.right = tips_ptr->base_ctrl.rect.left + tips_width - 1;
    tips_ptr->base_ctrl.rect.bottom = tips_ptr->base_ctrl.rect.top + tips_height - 1;

    // 如果超过要求绘制区域的情况
    if (tips_ptr->base_ctrl.rect.left  < orig_rect_ptr->left)
    {
        tips_ptr->base_ctrl.rect.left  = orig_rect_ptr->left;
    }

    if (tips_ptr->base_ctrl.rect.right > orig_rect_ptr->right)
    {
        tips_ptr->base_ctrl.rect.right = orig_rect_ptr->right;
    }

    // 设置text的区域
    tips_ptr->text_rect = tips_ptr->base_ctrl.rect;
    tips_ptr->text_rect.left += HORIZONTAL_GAP;     // 留出左右的边距
    tips_ptr->text_rect.right -= HORIZONTAL_GAP;

    return;
}

/*****************************************************************************/
//  Description : 设置tips最下端边界值，返回设置之前的值
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T CTRLTIPS_SetEdgeRect (
    MMI_HANDLE_T win_handle,
    GUI_RECT_T   edge_rect
)
{
    GUI_RECT_T      orig_edge_rect   = {0};
    TIPS_OBJ_T	*tips_ptr   = PNULL;

    tips_ptr = (TIPS_OBJ_T *) MMK_GetCtrlPtrByWin (win_handle, MMITHEME_GetTipsCtrlId());

    if (PNULL == tips_ptr)
    {
        return orig_edge_rect;
    }

    if (!TipsTypeOf ( (CTRLBASE_OBJ_T*) tips_ptr))
    {
        return orig_edge_rect;
    }

    orig_edge_rect = tips_ptr->style_info.edge_rect;

    tips_ptr->style_info.edge_rect = edge_rect;

    return orig_edge_rect;
}

/*****************************************************************************/
//  Description : tips update info
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void	CTRLTIPS_Update (
    MMI_HANDLE_T    win_handle,
    GUI_RECT_T	   *orig_rect_ptr,
    GUITIPS_DATA_T  *tips_data_ptr
)
{
    TIPS_OBJ_T	*tips_ptr = PNULL;

    tips_ptr = (TIPS_OBJ_T*) MMK_GetCtrlPtrByWin (win_handle, MMITHEME_GetTipsCtrlId());

    if (PNULL == tips_ptr || PNULL == tips_data_ptr)
    {
        return;
    }

    if (!TipsTypeOf ( (CTRLBASE_OBJ_T*) tips_ptr))
    {
        return;
    }

    // 获取字符串信息

    tips_ptr->text_info.wstr_len = MIN (tips_data_ptr->wstr_len, MMITIPS_TEXT_MAX_LEN);
    SCI_MEMSET (tips_ptr->text_info.wstr_ptr, 0, (MMITIPS_TEXT_MAX_LEN*sizeof (wchar)));

    MMIAPICOM_Wstrncpy (tips_ptr->text_info.wstr_ptr, tips_data_ptr->wstr_ptr, tips_ptr->text_info.wstr_len);
    // 计算区域
    TipsCalculateRect (tips_ptr, orig_rect_ptr);
    // draw tips
    TipsDrawBox (tips_ptr);

    return;
}

/*****************************************************************************/
//  Description : set lcd dev info
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void	CTRLTIPS_SetLcdInfo (
    MMI_HANDLE_T    win_handle,
    GUI_LCD_DEV_INFO *lcd_info_ptr
)
{
    TIPS_OBJ_T	*tips_ptr = PNULL;

    tips_ptr = (TIPS_OBJ_T*) MMK_GetCtrlPtrByWin (win_handle, MMITHEME_GetTipsCtrlId());

    if (PNULL == tips_ptr || PNULL == lcd_info_ptr)
    {
        return;
    }

    if (!TipsTypeOf ( (CTRLBASE_OBJ_T*) tips_ptr))
    {
        return;
    }

    tips_ptr->base_ctrl.lcd_dev_info = *lcd_info_ptr;

    return;
}

/*****************************************************************************/
//  Description : set tips text info
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	CTRLTIPS_SetTipsText (
    MMI_HANDLE_T    win_handle,
    GUITIPS_TEXT_T  tips_text
)
{
    BOOLEAN         result    = FALSE;
    TIPS_OBJ_T	*tips_ptr = PNULL;

    tips_ptr = (TIPS_OBJ_T*) MMK_GetCtrlPtrByWin (win_handle, MMITHEME_GetTipsCtrlId());

    if (PNULL == tips_ptr)
    {
        return result;
    }

    result = TRUE;

    if (!TipsTypeOf ( (CTRLBASE_OBJ_T*) tips_ptr))
    {
        return FALSE;
    }

    // 获取字符串信息
    tips_ptr->text_info.wstr_len = MIN (tips_text.wstr_len, MMITIPS_TEXT_MAX_LEN);
    SCI_MEMSET (tips_ptr->text_info.wstr_ptr, 0, (MMITIPS_TEXT_MAX_LEN*sizeof (wchar)));
    MMIAPICOM_Wstrncpy (tips_ptr->text_info.wstr_ptr, tips_text.wstr_ptr, tips_ptr->text_info.wstr_len);

    return result;
}

/*****************************************************************************/
//  Description : set tips valid
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	CTRLTIPS_SetTipsValid (
    MMI_HANDLE_T    win_handle,
    BOOLEAN         is_valid
)
{
    BOOLEAN         result    = TRUE;
    TIPS_OBJ_T	*tips_ptr = PNULL;

    tips_ptr = (TIPS_OBJ_T*) MMK_GetCtrlPtrByWin (win_handle, MMITHEME_GetTipsCtrlId());

    if ( (PNULL == tips_ptr) ||
            (!TipsTypeOf ( (CTRLBASE_OBJ_T*) tips_ptr)))
    {
        return FALSE;
    }

    result = tips_ptr->is_valid;
    tips_ptr->is_valid = is_valid;

    return result;
}

/*****************************************************************************/
//  Description : set tips font color
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T	CTRLTIPS_SetTextColor (
    MMI_HANDLE_T    win_handle,
    GUI_COLOR_T     text_color
)
{
    BOOLEAN         font_color    = 0;
    TIPS_OBJ_T	*tips_ptr = PNULL;

    tips_ptr = (TIPS_OBJ_T*) MMK_GetCtrlPtrByWin (win_handle, MMITHEME_GetTipsCtrlId());

    if ( (PNULL == tips_ptr) ||
            (!TipsTypeOf ( (CTRLBASE_OBJ_T*) tips_ptr)))
    {
        return 0;
    }

    font_color = tips_ptr->style_info.text_style.font_color;
    tips_ptr->style_info.text_style.font_color = text_color;

    return font_color;
}

/*****************************************************************************/
//  Description : get tips valid
//  Global resource dependence :
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	CTRLTIPS_GetTipsValid (
    MMI_HANDLE_T    win_handle
)
{
    BOOLEAN         result    = FALSE;
    TIPS_OBJ_T	*tips_ptr = PNULL;

    tips_ptr = (TIPS_OBJ_T*) MMK_GetCtrlPtrByWin (win_handle, MMITHEME_GetTipsCtrlId());

    if (PNULL == tips_ptr)
    {
        return result;
    }

    result = TRUE;

    if (!TipsTypeOf ( (CTRLBASE_OBJ_T*) tips_ptr))
    {
        return FALSE;
    }

    result = tips_ptr->is_valid;

    return result;
}

