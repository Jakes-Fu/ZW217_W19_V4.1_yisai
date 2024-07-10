/*! @file ctrlownerdraw_export.h
@brief 此文件介绍了ownerdraw控件的对外接口和数据类型
@author xiyuan.ma
@version 1.0
@date 07/23/2012
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 07/2012     xiyuan.ma             Creat
*******************************************************************************/

/*! @page page_OwnerDraw OwnerDraw

(暂无)

*******************************************************************************/

/*! @addtogroup ownerdrawGrp OwnerDraw
@brief ownerdraw模块
@details
    - 自绘控件,控件本身只显示背景和边框\n
    - 其他部分显示交给应用通过回调函数实现
@{
*******************************************************************************/

#ifndef _CTRLOWNERDRAW_EXPORT_H_
#define _CTRLOWNERDRAW_EXPORT_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//ownerdraw handle
typedef struct _ownerdraw_obj_tag* CTRL_OWNERDRAW_HANDLE_T;

/*! @struct  GUIOWNDRAW_INFO_T
@brief 自绘控件回调传出信息
*/
typedef struct
{
    GUI_RECT_T      ctrl_rect;      /*!<控件区域,不包括边框*/
    GUI_RECT_T      display_rect;   /*!<控件显示区域,不包括边框*/
    MMI_HANDLE_T    ctrl_handle;    /*!<控件handle*/
    MMI_HANDLE_T    win_handle;     /*!<控件所属窗口handle*/
} GUIOWNDRAW_INFO_T;

/*! @typedef GUIOWNDRAW_CALL_BACK_FUNC
@brief 自绘控件回调函数
*/
typedef void (*GUIOWNDRAW_CALL_BACK_FUNC) (GUIOWNDRAW_INFO_T *owner_draw_ptr);

typedef MMI_RESULT_E (*GUIOWNDRAW_TP_FUNC) (MMI_HANDLE_T ctrl_handle, GUI_POINT_T tp_point);
typedef MMI_RESULT_E (*GUIOWNDRAW_LOSE_FOCUS_FUNC) (MMI_HANDLE_T ctrl_handle);
typedef MMI_RESULT_E (*GUIOWNDRAW_GET_FOCUS_FUNC) (MMI_HANDLE_T ctrl_handle);

typedef struct
{
    GUIOWNDRAW_TP_FUNC      tp_down;
    GUIOWNDRAW_TP_FUNC      tp_short;
    GUIOWNDRAW_TP_FUNC      tp_move;
    GUIOWNDRAW_TP_FUNC      tp_long;
    GUIOWNDRAW_TP_FUNC      tp_up;

    GUIOWNDRAW_LOSE_FOCUS_FUNC  lose_focus;
    GUIOWNDRAW_GET_FOCUS_FUNC   get_focus;
} GUIOWNDRAW_TP_HANDLER_T;

/*! @struct  GUIOWNDRAW_INIT_DATA_T
@brief 自绘控件初始化数据
*/
typedef struct
{
    GUI_BOTH_RECT_T  both_rect;      /*!<控件横竖屏区域*/
    GUIOWNDRAW_CALL_BACK_FUNC OwnDrawFunc;    /*!<控件回调函数*/
} GUIOWNDRAW_INIT_DATA_T;
/*--------------------------------------------------------------------------
                          FUNCTION DEFINITION
--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 设置TP处理函数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLOWNERDRAW_SetCallback (
    MMI_CTRL_ID_T   ctrl_id,//in
    GUIOWNDRAW_TP_HANDLER_T *tp_handler_ptr
);

/*****************************************************************************/
//  Description : 设置用户数据
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLOWNERDRAW_SetAddData (
    MMI_CTRL_ID_T   ctrl_id,//in
    ADD_DATA        add_data
);

/*****************************************************************************/
//  Description : 获取用户数据
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_GetAddData (
    MMI_CTRL_ID_T   ctrl_id,//in
    ADD_DATA        *add_data_ptr
);

/*****************************************************************************/
// 	Description : set owner draw background
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_SetBg (
    MMI_CTRL_ID_T   ctrl_id,//in
    GUI_BG_T        *bg_ptr //in
);

/***************************************************************************//*!
@brief 设置自绘控件指定高度
@author Jassmine
@param ctrl_handle [in] 控件handle
@param height      [in] 高度
@return 操作是否成功
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_SetHeight (
    MMI_HANDLE_T    ctrl_handle,
    uint16          height
);

/***************************************************************************//*!
@brief 刷新自绘控件
@author Jassmine
@param ctrl_id [in] 控件id
@return 操作是否成功
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_Update (
    MMI_HANDLE_T   ctrl_id
);

/***************************************************************************//*!
@brief 获得自绘控件区域
@author Jassmine
@param ctrl_id  [in]     控件id
@param rect_ptr [in/out] 区域
@return 操作是否成功
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_GetRect (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    GUI_RECT_T        *rect_ptr   //in/out:
);

/*****************************************************************************/
//  Description : set owner draw rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLOWNERDRAW_SetRect (
    MMI_HANDLE_T    ctrl_id,
    GUI_RECT_T      *rect_ptr
);

/***************************************************************************//*!
@brief 获得自绘控件显示区域
@author Jassmine
@param ctrl_id  [in]     控件id
@param rect_ptr [in/out] 显示区域
@return 操作是否成功
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_GetDisplayRect (
    MMI_CTRL_ID_T  ctrl_id,    //in:
    GUI_RECT_T     *rect_ptr   //in/out:
);

/***************************************************************************//*!
@brief 设置自绘控件是否允许显示边框
@author Jassmine
@param ctrl_id   [in] 控件id
@param is_permit [in]是否允许显示
@return 操作是否成功
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_PermitBorder (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN          is_permit
);

/*****************************************************************************/
//  Description : set ownerdraw is visible
//  Global resource dependence :
//  Author: hongjun.jia
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_SetVisible (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_visible,
    BOOLEAN         is_update
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_CTRLOWNERDRAW_EXPORT_H_
