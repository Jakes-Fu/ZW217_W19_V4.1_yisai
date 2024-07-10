/*! @file ctrlownerdraw_export.h
@brief ���ļ�������ownerdraw�ؼ��Ķ���ӿں���������
@author xiyuan.ma
@version 1.0
@date 07/23/2012
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 07/2012     xiyuan.ma             Creat
*******************************************************************************/

/*! @page page_OwnerDraw OwnerDraw

(����)

*******************************************************************************/

/*! @addtogroup ownerdrawGrp OwnerDraw
@brief ownerdrawģ��
@details
    - �Ի�ؼ�,�ؼ�����ֻ��ʾ�����ͱ߿�\n
    - ����������ʾ����Ӧ��ͨ���ص�����ʵ��
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
@brief �Ի�ؼ��ص�������Ϣ
*/
typedef struct
{
    GUI_RECT_T      ctrl_rect;      /*!<�ؼ�����,�������߿�*/
    GUI_RECT_T      display_rect;   /*!<�ؼ���ʾ����,�������߿�*/
    MMI_HANDLE_T    ctrl_handle;    /*!<�ؼ�handle*/
    MMI_HANDLE_T    win_handle;     /*!<�ؼ���������handle*/
} GUIOWNDRAW_INFO_T;

/*! @typedef GUIOWNDRAW_CALL_BACK_FUNC
@brief �Ի�ؼ��ص�����
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
@brief �Ի�ؼ���ʼ������
*/
typedef struct
{
    GUI_BOTH_RECT_T  both_rect;      /*!<�ؼ�����������*/
    GUIOWNDRAW_CALL_BACK_FUNC OwnDrawFunc;    /*!<�ؼ��ص�����*/
} GUIOWNDRAW_INIT_DATA_T;
/*--------------------------------------------------------------------------
                          FUNCTION DEFINITION
--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : ����TP������
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLOWNERDRAW_SetCallback (
    MMI_CTRL_ID_T   ctrl_id,//in
    GUIOWNDRAW_TP_HANDLER_T *tp_handler_ptr
);

/*****************************************************************************/
//  Description : �����û�����
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLOWNERDRAW_SetAddData (
    MMI_CTRL_ID_T   ctrl_id,//in
    ADD_DATA        add_data
);

/*****************************************************************************/
//  Description : ��ȡ�û�����
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
@brief �����Ի�ؼ�ָ���߶�
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param height      [in] �߶�
@return �����Ƿ�ɹ�
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_SetHeight (
    MMI_HANDLE_T    ctrl_handle,
    uint16          height
);

/***************************************************************************//*!
@brief ˢ���Ի�ؼ�
@author Jassmine
@param ctrl_id [in] �ؼ�id
@return �����Ƿ�ɹ�
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_Update (
    MMI_HANDLE_T   ctrl_id
);

/***************************************************************************//*!
@brief ����Ի�ؼ�����
@author Jassmine
@param ctrl_id  [in]     �ؼ�id
@param rect_ptr [in/out] ����
@return �����Ƿ�ɹ�
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
@brief ����Ի�ؼ���ʾ����
@author Jassmine
@param ctrl_id  [in]     �ؼ�id
@param rect_ptr [in/out] ��ʾ����
@return �����Ƿ�ɹ�
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLOWNERDRAW_GetDisplayRect (
    MMI_CTRL_ID_T  ctrl_id,    //in:
    GUI_RECT_T     *rect_ptr   //in/out:
);

/***************************************************************************//*!
@brief �����Ի�ؼ��Ƿ�������ʾ�߿�
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param is_permit [in]�Ƿ�������ʾ
@return �����Ƿ�ɹ�
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
