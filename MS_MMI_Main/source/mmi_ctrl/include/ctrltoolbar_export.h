/*! @file guitoolbar.h
@brief ���ļ�������toolbar�ؼ��Ķ���ӿں���������
@author James.Zhang
@version 1.0
@date 2011/02/18
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 9/2010         James.Zhang      Create
*******************************************************************************/

/*! @page page_ToolBar ToolBar

(����)

*******************************************************************************/

/*! @addtogroup ToolbarGrp ToolBar
@brief Toolbar�ؼ�
@details Toolbar�ؼ���Щ����menu��Toolbarʹ��һ�������ε�С�������ڣ�����һЩ����ѡ�
@{
*******************************************************************************/

#ifndef _CTRLTOOLBAR_EXPORT_H_
#define _CTRLTOOLBAR_EXPORT_H_ 

/*--------------------------------------------------------------------------
                          Include Files                               
 --------------------------------------------------------------------------*/

#include "mmk_type.h"
#include "guifont.h"

#include "ctrlbase_export.h"

/*--------------------------------------------------------------------------
                         Compiler Flag                                   
 --------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------
                         Macro Declaration                                                                   
 --------------------------------------------------------------------------*/  
/*! @def GUITOOLBAR_INVALID_INDEX
@brief ��Ч������ֵ
*/
#define GUITOOLBAR_INVALID_INDEX            (0xffff)

/*--------------------------------------------------------------------------
                         Data Structures                                  
 --------------------------------------------------------------------------*/
// toolbar handle
typedef struct _toolbar_obj_tag* CTRL_TOOLBAR_HANDLE_T;

/*! @struct GUITOOLBAR_INIT_DATA_T
@brief  ��ʼ����
*/
typedef struct
{
    GUI_BOTH_RECT_T both_rect;/*!< ����*/ 
} GUITOOLBAR_INIT_DATA_T;


/*! @enum GUITOOLBAR_ITEM_TYPE_E
@brief  ö��item ����
*/
typedef enum
{
    GUITOOLBAR_ITEM_NONE, /*!< ��*/ 
    GUITOOLBAR_ITEM_IMAGE_ID,/*!< ͼƬ*/      
    GUITOOLBAR_ITEM_TEXT_ID,/*!< �ַ�*/    
    GUITOOLBAR_ITEM_BOTH_ID/*!< ͼƬ���ַ�*/    
}GUITOOLBAR_ITEM_TYPE_E;

/*! @struct GUITOOLBAR_ITEM_T
@brief item �ṹ
*/
typedef struct
{
    uint32                  user_data;   /*!< �û��Զ������� */
    GUITOOLBAR_ITEM_TYPE_E  type;/*!< item ���� */
    MMI_IMAGE_ID_T          image_id;    /*!< ��ԴͼƬid */
    MMI_IMAGE_ID_T          image_gray; /*!< �һ�ͼ */
    MMI_TEXT_ID_T           text_id;    /*!< ��Դ�ַ���id */
}GUITOOLBAR_ITEM_T;

/*! @enum GUITOOLBAR_ANIM_DIRECTION_E
@brief   ö��Toolbar�Ķ�������
*/
typedef enum  
{
    GUITOOLBAR_DIR_INVALID      = 0x00000000,   /*!< ��Ч*/    
    
    GUITOOLBAR_DIR_UP      = 0x00000001,   /*!< ��*/    
    GUITOOLBAR_DIR_DOWN    = 0x00000002,   /*!< ��*/    
    GUITOOLBAR_DIR_VERTICAL     = 0x0000000f,/*!< ��ֱ*/    

    GUITOOLBAR_DIR_RIGHT     = 0x00000010,   /*!< ��*/    
    GUITOOLBAR_DIR_LEFT      = 0x00000020,   /*!< ��*/    
    GUITOOLBAR_DIR_HORIZONTAL   = 0x000000f0/*!< ˮƽ*/    
}GUITOOLBAR_ANIM_DIRECTION_E;

/*! @enum  GUITOOLBAR_ANIM_ACTION_E 
@brief   ö��Toolbar�Ķ�������
*/
typedef enum
{
    GUITOOLBAR_ACT_NONE,         /*!< ��*/
    GUITOOLBAR_ACT_SHRINK,      /*!< ����Toolbar*/
    GUITOOLBAR_ACT_EXPAND       /*!< չ�� Toolbar*/
}GUITOOLBAR_ANIM_ACTION_E;

/*! @enum  GUITOOLBAR_SHOW_TYPE_E
@brief   ö��Toolbar����ʾ����
*/
typedef enum
{
    GUITOOLBAR_INVALID,     /*!< ��Ч*/
    GUITOOLBAR_SHRINK,      /*!< ����*/
    GUITOOLBAR_EXPAND,      /*!< չ��*/

    GUITOOLBAR_TYPE_MAX   
}GUITOOLBAR_SHOW_TYPE_E;

/*! @typedef TOOLBAR_PRE_EXPAND_FUNC
@brief Toolbar�ؼ���չ���ܵĺ���ָ��
*/
typedef void (*TOOLBAR_PRE_EXPAND_FUNC)( 
                                        MMI_HANDLE_T        ctrl_handle,
                                        void*               param_ptr
                                        );


/*--------------------------------------------------------------------------
                        Function Declare                             
 --------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : create dynamic toolbar
//  Global resource dependence : 
//  Author: cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLTOOLBAR_CreateDynamic(
                                             MMI_WIN_ID_T            win_id,     //in:
                                             MMI_CTRL_ID_T           ctrl_id,    //in:
                                             GUITOOLBAR_INIT_DATA_T   *init_ptr   //in:
                                             );

/***************************************************************************//*!
@brief ����Toolbar�ؼ�������item
@author James.Zhang
@param ctrl_handle [in] Toolbar�ؼ����
@param item_ptr [in] Toolbar�ؼ���item��Դ��ָ��
@param total_num [in] ��Ŀ����
@retval TRUE ���óɹ�
@retval FALSE ����ʧ��
@return �����Ƿ����óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetAllItem(
                                     MMI_HANDLE_T             ctrl_handle,
                                     const GUITOOLBAR_ITEM_T* item_ptr,
                                     uint16                   total_num
                                     );


/***************************************************************************//*!
@brief ��ȡ���Toolbar�ؼ�item��index
@author James.Zhang
@param ctrl_handle [in] Toolbar�ؼ����
@return ����ѡ��item������ֵ
@note 
*******************************************************************************/
PUBLIC uint16 CTRLTOOLBAR_GetClickItemIndex(
                                           MMI_HANDLE_T ctrl_handle
                                           );


/***************************************************************************//*!
@brief ��ȡitem����Դָ��
@author James.Zhang
@param ctrl_handle [in] Toolbar�ؼ����
@param index [in] item����ֵ
@return ����item��Դ��ָ��
@note 
*******************************************************************************/
PUBLIC const GUITOOLBAR_ITEM_T* CTRLTOOLBAR_GetItemPtr(
                                                      MMI_HANDLE_T ctrl_handle,
                                                      uint16       index
                                                      );


/***************************************************************************//*!
@brief ��ȡitem���û��Զ�������
@author cheng.luo
@param ctrl_handle [in] Toolbar�ؼ����
@param index [in] item����ֵ
@return �����û��Զ�������
@note 
*******************************************************************************/
PUBLIC uint32 CTRLTOOLBAR_GetItemUserDate(
                                         MMI_HANDLE_T ctrl_handle,
                                         uint16       index
                                         );


/***************************************************************************//*!
@brief ����item�Ƿ���
@author cheng.luo
@param ctrl_handle [in] Toolbar�ؼ����
@param index [in] item����ֵ
@param is_gray [in] �Ƿ���ʾ���(��Ч)
@note 
*******************************************************************************/
PUBLIC void CTRLTOOLBAR_SetItemGray(
                                   MMI_HANDLE_T ctrl_handle,
                                   uint16       index,
                                   BOOLEAN      is_gray
                                   );


/***************************************************************************//*!
@brief ��ȡToolbar�ؼ�����ʾ���� 
@author cheng.luo
@param ctrl_handle [in] Toolbar�ؼ����
@return ����Toolbar�ؼ�����ʾ����
@note ��ʾ���Ͱ���(��Ч��������չ��)
*******************************************************************************/
PUBLIC GUITOOLBAR_SHOW_TYPE_E CTRLTOOLBAR_GetShowType(
                                                      MMI_HANDLE_T ctrl_handle
                                                      );


/***************************************************************************//*!
@brief ����Toolbar�ؼ�����ʾ���� 
@author cheng.luo
@param ctrl_handle [in] Toolbar�ؼ����
@param show_type  [in] Toolbar�ؼ�����ʾ����
@note ��ʾ���Ͱ���(��Ч��������չ��)
*******************************************************************************/
PUBLIC void CTRLTOOLBAR_SetShowType(
                                   MMI_HANDLE_T             ctrl_handle,
                                   GUITOOLBAR_SHOW_TYPE_E   show_type
                                   );


/***************************************************************************//*!
@brief ����Toolbar�ؼ����Ƿ���ʾ
@author cheng.luo
@param ctrl_handle [in] Toolbar�ؼ����
@param is_visible  [in] Toolbar�ؼ��Ƿ���ʾ�ɼ�
@note 
*******************************************************************************/
PUBLIC void CTRLTOOLBAR_SetVisible(
                                  MMI_HANDLE_T             ctrl_handle,
                                  BOOLEAN                  is_visible
                                  );


/***************************************************************************//*!
@brief ����Toolbar�ؼ��Ƿ��Զ�����
@author cheng.luo
@param ctrl_handle [in] Toolbar�ؼ����
@param is_open  [in] Toolbar�ؼ��Ƿ��Զ�����
@note 
*******************************************************************************/
PUBLIC void CTRLTOOLBAR_SetAutoShrink(
                                     MMI_HANDLE_T   ctrl_handle,
                                     BOOLEAN        is_open
                                     );


/***************************************************************************//*!
@brief ����Toolbar�ؼ��Զ�������ʱ��
@author cheng.luo
@param ctrl_handle [in] Toolbar�ؼ����
@param interval  [in] Toolbar�ؼ��Զ������ļ�϶ʱ��
@retval TRUE ���óɹ�
@retval FALSE ����ʧ��
@return �����Ƿ����óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetAutoShrinkInterval(
                                                MMI_HANDLE_T ctrl_handle,
                                                uint32        interval
                                                );


/***************************************************************************//*!
@brief ����Toolbar�ؼ�����չ����
@author cheng.luo
@param ctrl_handle [in] Toolbar�ؼ����
@param func_ptr [in] Toolbar�ؼ�����չ���ܵĺ���ָ��
@param param_ptr [in] Toolbar�ؼ�����չ���ܵĲ�����ָ��
@return Toolbar�ؼ��ľɵ���չ���ܵĺ���ָ��
@note 
*******************************************************************************/
PUBLIC TOOLBAR_PRE_EXPAND_FUNC CTRLTOOLBAR_SetPreExpandFunc(
                                                           MMI_HANDLE_T                ctrl_handle,
                                                           TOOLBAR_PRE_EXPAND_FUNC     func_ptr,
                                                           void                        *param_ptr
                                                           );


/***************************************************************************//*!
@brief ����Toolbar�ؼ�ͼ��Ŀ�Ⱥ͸߶�
@author cheng.luo
@param ctrl_handle [in] Toolbar�ؼ����
@param width_ptr [in] Toolbar�ؼ���ͼ���ȵ�ָ��
@param height_ptr [in] Toolbar�ؼ���ͼ��߶ȵ�ָ��
@retval TRUE ���óɹ�
@retval FALSE ����ʧ��
@return �����Ƿ����óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_GetIconWidthHeight(
                                             MMI_HANDLE_T    ctrl_handle,
                                             uint16          *width_ptr,
                                             uint16          *height_ptr
                                             );


/***************************************************************************//*!
@brief ����Toolbar�ؼ���λ��
@author cheng.luo
@param ctrl_handle [in] Toolbar�ؼ����
@param rect_ptr  [in] Toolbar�ؼ���ͼ��λ�õ�ָ��
@param is_update [in] Toolbar�ؼ���ͼ���Ƿ����
@retval TRUE ���óɹ�
@retval FALSE ����ʧ��
@return �����Ƿ����óɹ�
@note 
-# ���ѡ����½�������Ϣ�����ػ�Toolbar�ؼ���
-# ���û��ѡ��������ӳ��ػ档
*******************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetIconRect(
                                      MMI_HANDLE_T              ctrl_handle,
                                      const GUI_RECT_T          *rect_ptr,
                                      BOOLEAN                   is_update
                                      );


/***************************************************************************//*!
@brief ����Toolbar�ؼ��Ļ�׼��(��Ϊ���µ�)
@author cheng.luo
@param ctrl_handle [in] Toolbar�ؼ����
@param pt_ptr  [in] Toolbar�ؼ������µ������ָ��
@param is_update [in] Toolbar�ؼ��Ƿ����
@retval TRUE ���óɹ�
@retval FALSE ����ʧ��
@return �����Ƿ����óɹ�
@note 
-# ���ѡ����½�������Ϣ�����ػ�Toolbar�ؼ���
-# ���û��ѡ��������ӳ��ػ档
*******************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetPanelBasePt(
                                         MMI_HANDLE_T              ctrl_handle,
                                         const GUI_POINT_T         *pt_ptr,
                                         BOOLEAN                   is_update
                                         );


/***************************************************************************//*!
@brief ����Toolbar�ؼ��Ķ�������
@author cheng.luo
@param ctrl_handle [in] Toolbar�ؼ����
@param direction [in] �ؼ������ķ���
@retval TRUE ���óɹ�
@retval FALSE ����ʧ��
@return �����Ƿ����óɹ�
@note �ؼ������ķ���(�����������ң�ˮƽ����ֱ)
*******************************************************************************/
PUBLIC BOOLEAN CTRLTOOLBAR_SetPanelPopForm(
                                          MMI_HANDLE_T                   ctrl_handle,
                                          GUITOOLBAR_ANIM_DIRECTION_E    direction
                                          );

/*--------------------------------------------------------------------------
                         Compiler Flag                                   
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
