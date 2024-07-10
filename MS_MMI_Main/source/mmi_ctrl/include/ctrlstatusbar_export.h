/*****************************************************************************
** File Name:      ctrlstatusbar_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2012     xiyuan.ma              Creat
******************************************************************************/

#ifndef _CTRLSTATUSBAR_EXPORT_H_
#define _CTRLSTATUSBAR_EXPORT_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"


#include "guicommon.h"
#include "guictrl.h"
#include "mmitheme_statusbar.h"
#include "mmitheme_pos.h"
#include "ui_layer.h"
#include "caf.h"
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
/*! @typedef GUISTATUSBAR_STATE_T
@brief �ؼ���״̬
*/
typedef uint32 GUISTATUSBAR_STATE_T;


/*! @def GUISTATUSBAR_STATE_INVALID
@brief �ؼ���Ч
*/
#define GUISTATUSBAR_STATE_INVALID 0x00000001 // �ؼ���Ч


// �ؼ��Ƿ�֧�ֻص�, ״̬��ÿ��itemҲ��һ����������item�����Ƿ�
// ִ�лص�������״̬������ִ�и�״̬���������״̬Ϊfalse��ʱ��
// ��ʹitem����������Ч��Ҳ����ִ�У�item�������õĺ���ΪGUIWIN_SetItemHandleFuncValid
// ��״̬Ĭ��ΪFALSE
/*! @def GUISTATUSBAR_STATE_EVENT
@brief �ؼ��Ƿ�֧�ֻص�, ״̬��ÿ��itemҲ��һ����������item�����Ƿ�ִ�лص�������״̬������ִ�и�״̬��
�������״̬Ϊfalse��ʱ�򣬼�ʹitem����������Ч��Ҳ����ִ�У�item�������õĺ���ΪGUIWIN_SetItemHandleFuncValid��״̬Ĭ��ΪFALSE
*/
#define GUISTATUSBAR_STATE_EVENT 0x00000002

#define GUISTATUSBAR_STATE_USE_LAYER 0x00000008 // use stb layer

#define GUISTATUSBAR_STATE_SCROLLMSG_PAUSE 0x00000010 // pause msg

#define GUISTATUSBAR_STATE_CUSTOMER_LAYER 0x00000004 // customer stb layer

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*! @enum  GUISTATUSBAR_TYPE_E
@brief  item show style, Ŀǰֻ֧��ˮƽ��ʾ���Ժ���չ
*/
typedef enum
{
    GUISTATUSBAR_TYPE_NONE = 0, /*!< ��*/
    GUISTATUSBAR_TYPE_HOR,  		/*!< ˮƽ��ʾ*/
    GUISTATUSBAR_TYPE_MAX				/*!< ϵͳ����*/
} GUISTATUSBAR_TYPE_E;


/*! @struct GUISTATUSBAR_INIT_DATA_T
@brief ״̬����ʼ������
*/
typedef struct
{
    GUI_BOTH_RECT_T           both_rect;   /*!< �ؼ��ĺ���������*/
} GUISTATUSBAR_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                                                                                        *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief ����statusbar
@author xiyuan.ma
@param param_ptr  [in] ��ʼ������
@param ictrl_ptr  [in] CAF�ؼ�����
@return �ؼ�handle
@note
*******************************************************************************/
// PUBLIC CTRL_STATUSBAR_HANDLE_T CTRLSTATUSBAR_Create (
//     void *param_ptr,
//     IGUICTRL_T *ictrl_ptr
// );


/***************************************************************************//*!
@brief ����statusbar
@author xiyuan.ma
@param label_handle   [in] �ؼ����
@return �Ƿ�ɹ�
@note
*******************************************************************************/
// PUBLIC BOOLEAN CTRLSTATUSBAR_Destroy (
//     CTRL_STATUSBAR_HANDLE_T   statusbar_handle
// );


/***************************************************************************//*!
@brief ��̬����״̬���ؼ�
@author xiaoqing.lu
@param win_handle [in] �ؼ��ĸ�����handle��id
@retval true/false
@return �����ɹ���ʧ��
@note ʹ�øú�������̬����ʱ��ϵͳ��ʹ��ͳһ�Ŀؼ�id��MMICOMMON_STATUSBAR_CTRL_ID���ؼ�������ʹ��Ĭ�����򣨴��ڶ�����
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_CreateCtrlDyn (
    MMI_HANDLE_T win_handle
);


/***************************************************************************//*!
@brief ���ÿؼ��ı���
@author xiaoqing.lu
@param win_handle [in] �ؼ��ĸ�����handle��id
@param image_id [in] ͼƬ��ԴID
@retval void
@return void
@note ��������ã���ʹ��Ĭ�ϱ���
*******************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetBgImageId (
    MMI_HANDLE_T win_handle,
    MMI_IMAGE_ID_T image_id
);


/***************************************************************************//*!
@brief
@author xiaoqing.lu
@param win_handle [in] �ؼ��ĸ�����handle��id
@param bg_info [in] ͼƬ������Ϣ
@retval void
@return void
@note ��������ã���ʹ��Ĭ�ϱ���
*******************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetBg (
    MMI_HANDLE_T win_handle,
    GUI_BG_T  bg_info
);


/***************************************************************************//*!
@brief post messages of status bar
@author xiaoqing.lu
@param win_handle [in] �ؼ��ĸ�����handle��id
@param is_full_paint [in] �Ƿ�ˢ����������
@retval true/false
@return �ɹ���ʧ��
@note ���ؼ�����ʾ�Լ��ı���ʱ����Ҫˢ�´��ڵı�����is_full_paint=true��
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLSTATUSBAR_UpdateCtrl (
    MMI_HANDLE_T win_handle,
    BOOLEAN is_full_paint
);


/***************************************************************************//*!
@brief �����Ƿ���ʾ�ؼ�������
@author xiaoqing.lu
@param win_handle [in] �ؼ��ĸ�����handle��id
@param is_desplay_bg [in] �Ƿ���ʾ�ؼ�������
@retval void
@return void
@note ���ؼ�����ʾ������ʱ��ˢ�¿ؼ���ˢ����������
*******************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetBgIsDesplay (
    MMI_HANDLE_T win_handle,
    BOOLEAN is_desplay_bg
);


/*****************************************************************************//*!
@brief      �����Ƿ����״̬�����û�����
@author   xiyuan.ma
@param   win_handle              [in] �ؼ��ĸ�����handle��id
@param   clear_customer_bg   [in]
@retval    void
@return    void
@note      TRUEʱ,ˢ�±�������û�����
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_ClearCustomerBg (
    MMI_HANDLE_T win_handle,
    BOOLEAN clear_customer_bg
);


/***************************************************************************//*!
@brief  ��̬����״̬���ؼ����û������Զ���ؼ�������
@author xiaoqing.lu
@param win_handle [in] �ؼ��ĸ�����handle��id
@param rect_ptr [in] �ؼ�����������
@retval true/false
@return �ɹ���ʧ��
@note �ú����� \link CTRLSTATUSBAR_CustomCreate() \endlink ���ƣ����������û����Զ�������
*******************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_CustomCreate (
    MMI_HANDLE_T win_handle,
    GUI_BOTH_RECT_T *rect_ptr
);


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetCommonBg (
    MMI_HANDLE_T win_handle,
    GUI_BG_T  bg_info
);


/***************************************************************************//*!
@brief ���ÿؼ�״̬
@author xiaoqing.lu
@param win_handle [in] �ؼ��ĸ�����handle��id
@param ctrl_state [in] �ؼ�״̬
@param is_set true [in] ��ʾ���ã�false��ʾȡ��
@retval void
@return void
@note
*******************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetState (
    MMI_HANDLE_T win_handle,
    uint32 ctrl_state,
    BOOLEAN is_set
);


/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_GetState (
    MMI_HANDLE_T win_handle,
    uint32 ctrl_state
);


/*****************************************************************************/
//  Description : ����item��bg
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_SetItemBg (
    uint32            item_index,
    GUI_BG_T          *bg_info_ptr
);


/*****************************************************************************/
//  Description : �û���ȡstatusbar�Ĳ���Ϣ
//  Global resource dependence :
//  Author:xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_GetCtrlLayer (
    MMI_HANDLE_T win_handle,
    GUI_LCD_DEV_INFO *dev_info_ptr
);


#if defined PDA_UI_DROPDOWN_WIN || defined DROPDOWN_NOTIFY_SHORTCUT
/*****************************************************************************/
//  Description : Set Status Bar DropDown State
//  Global resource dependence : none
//  Author: Paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetDropDownState (
    MMI_HANDLE_T win_handle,
    BOOLEAN enable
);


/*****************************************************************************/
//  Description : get Status Bar DropDown State
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_IsDropDown (
    MMI_HANDLE_T win_handle
);


/*****************************************************************************/
//  Description : Set Status Bar DropDown State
//  Global resource dependence : none
//  Author: Paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTATUSBAR_SetPressed (
    MMI_HANDLE_T win_handle,
    BOOLEAN is_true
);


/*****************************************************************************/
//  Description : get Status Bar DropDown State
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTATUSBAR_IsPressed (
    MMI_HANDLE_T win_handle
);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

/*@}*/
