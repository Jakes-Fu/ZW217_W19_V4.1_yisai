/*****************************************************************************
** File Name:      ctrlim.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLIM_H_
#define _CTRLIM_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlim_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
// im type info
#define CTRL_IM_TYPE     (IM_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// im init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;
    GUIIM_LANGUAGE_T lang_id; /*!< ��ʼ���������� */
    GUIIM_INPUT_TYPE_T input_type; /*!< ��ʼ�����뷽ʽ */
    GUIIM_TYPE_T im_type; /*!< ��ʼ�����뷨 */
    GUIIM_CAPITAL_MODE_T cap_mode; /*!< ��ʼ����Сдģʽ */
    
    GUIIM_INPUT_TYPE_T input_type_set; /*!< �������뷽ʽ���� */
    GUIIM_TYPE_T im_type_set; /*!< �������뷨���� */
    
    BOOLEAN is_ban_symbol; /*!< �Ƿ��ֹ�������� */
    GUIIM_TAG_T tag; /*!< ���뷨�ĸ������� */

    GUIIM_LANGUAGE_T set_input_lang; /*!< �������뷨Ĭ������ */
    
    GUIIM_GetContextFunction_T f_get_context; /*!< ��ȡ�����ĺ��� */
    GUIIM_GetCapacityFunction_T f_get_capacity; /*!< ��ȡ�����ַ��������� */

    // self inif param ...
} CTRLIM_INIT_PARAM_T;

//im control
typedef struct 
{
    CTRLBASE_OBJ_T              base_ctrl;
    
    MMI_HANDLE_T win_handle; /*!< 
���ǵ�ƽ̨���ؼ��ǻ��ڴ��ڵģ����Ա���̳и��ؼ����߸����� */
    MMI_WIN_ID_T win_id; /*!< ����id��ƽ̨��������Ϣ */

    GUIIM_INIT_PARAM_T init_param; /*!< �����ʼ������ */
    
    void* im_handle; /*!< ��ǰ���뷨Ӧ�õ�ʵ������ */
    GUIIM_APIS_PTR_T apis_ptr; /*!< ��ǰ���뷨Ӧ�õĺ����б���� */

    GUIIM_METHOD_PARAM_T method_param; /*!< ���뷨���õĲ��� */
    BOOLEAN is_default_method; /*!< 
����ǻ�ȡ��Ĭ�����뷨������Ҫ����Ӧ���趨 */
    
    GUIIM_NOTIFY_QUEUE_T notify; /*!< ������Ϣ���� */
    GUIIM_COMMIT_DATA_T last_commit; /*!< ����ύ��Ϣ������������Ϣ���� */
    GUIIM_EDIT_STATE_T edit_state_bak; /*!< ���뷨״̬���Ʊ�־���û�����
softkey�� */

    GUIIM_CUSTOM_KEY_PARAM_T *custom_key_ptr; /*!< ���ư�����Ϣ */

    GUIIM_APPEND_WIN_NODE_T *append_win_ptr; /*!< ���뷨�ؼ���ش��ڱ� */
    BOOLEAN is_msg_band;

} CTRLIM_OBJ_T;

// im class for public or protected method
typedef struct _im_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLIM_VTBL_T;
/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get im type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T IM_GetType(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
