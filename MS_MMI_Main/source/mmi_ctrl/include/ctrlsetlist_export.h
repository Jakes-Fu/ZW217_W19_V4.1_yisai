/*****************************************************************************
** File Name:      ctrlsetlist_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Creat
******************************************************************************/

#ifndef _CTRLSETLIST_EXPORT_H_
#define _CTRLSETLIST_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "ctrlsetlist_export.h"
#include "guicommon.h"
#include "guictrl.h"
#include "mmi_link.h"
#include "mmitheme_setlist.h"

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


/*! @def GUISETLIST_INVALID_ITEM_INDEX
@brief ��Ч��item������
*/
#define GUISETLIST_INVALID_ITEM_INDEX  0xFFFFFFFF // ��Ч��item������

/*! @typedef GUISETLIST_ITEM_STATE_T
@brief ����item״̬
*/
typedef uint32 GUISETLIST_ITEM_STATE_T;
/*! @def GUISETLIST_ITEM_STATE_NONE
@brief null state
*/
#define GUISETLIST_ITEM_STATE_NONE          0x00000000 // null state
/*! @def GUISETLIST_ITEM_STATE_TEXT
@brief item��ʾ�ı�
*/
#define GUISETLIST_ITEM_STATE_TEXT          0x00000001 // item��ʾ�ı�
/*! @def GUISETLIST_ITEM_STATE_IMAGE
@brief item��ʾͼƬ
*/
#define GUISETLIST_ITEM_STATE_IMAGE         0x00000002 // item��ʾͼƬ
/*! @def GUISETLIST_ITEM_STATE_INVISIBLE
@brief item���ɼ�,Ĭ�Ͽɼ�
*/
#define GUISETLIST_ITEM_STATE_INVISIBLE     0x00000004 // item���ɼ�,Ĭ�Ͽɼ�

/*! @typedef GUISETLIST_STATE_T
@brief ����ؼ���״̬
*/
typedef uint32 GUISETLIST_STATE_T;
/*! @def GUISETLIST_STATE_NONE
@brief null state
*/
#define GUISETLIST_STATE_NONE               0x00000000 // null state
/*! @def GUISETLIST_STATE_ISCYC
@brief �Ƿ�ѭ��������Ĭ�ϲ�ѭ��
*/
#define GUISETLIST_STATE_ISCYC              0x00000001 // �Ƿ�ѭ��������Ĭ�ϲ�ѭ��
/*! @def GUISETLIST_STATE_USER_OPEN
@brief �û��Լ��򿪴���
*/
#define GUISETLIST_STATE_USER_OPEN          0x00000002 // �û��Լ��򿪴���
/*! @def GUISETLIST_STATE_OPEN_POPWIN
@brief �Զ���pop list win
*/
#define GUISETLIST_STATE_OPEN_POPWIN        0x00000004 // �Զ���pop list win
/*! @def GUISETLIST_STATE_OPEN_FULLWIN
@brief �Զ���pop list win
*/
#define GUISETLIST_STATE_OPEN_FULLWIN       0x00000008 // �Զ���pop list win
/*! @def GUISETLIST_STATE_HAS_TITLE
@brief  �Ƿ���Title
*/
#define GUISETLIST_STATE_HAS_TITLE          0x00000010 // �Ƿ���Title
/*! @def GUISETLIST_STATE_LEFTSK_SWITCH
@brief left softkey��Ӧ��web����ͬ���л���Ĭ��ΪFALSE
*/
#define GUISETLIST_STATE_LEFTSK_SWITCH      0x00000020 // left softkey��Ӧ��web����ͬ���л���Ĭ��ΪFALSE

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @enum GUISETLIST_DATA_TYPE_E
@brief ����ؼ����ݵ�����
*/
typedef enum
{
    GUISETLIST_DATA_NONE,		/*!< ��*/
    GUISETLIST_DATA_TEXT,       /*!< �ı�*/ 
    GUISETLIST_DATA_IMAGE_ID,   /*!< ͼƬid*/
    GUISETLIST_DATA_ANIM_DATA,  /*!< ͼƬ����*/ 
    GUISETLIST_DATA_ANIM_PATH,  /*!< ͼƬ·��*/
    GUISETLIST_DATA_MAX,
}GUISETLIST_DATA_TYPE_E;

/*! @enum GUISETLIST_TYPE_E
@brief ����ؼ���ʾ����
*/
typedef enum
{
    GUISETLIST_TYPE_DEFAULT,
    GUISETLIST_TYPE_RADIOLIST = GUISETLIST_TYPE_DEFAULT, /*!<  ��ѡ(Radio)���� */
    GUISETLIST_TYPE_CHECKLIST,                           /*!<  ��ѡ(check)���� */
}GUISETLIST_TYPE_E;

/*! @union GUISETLIST_DATA_U
@brief ����ͼƬ����������
*/
typedef union
{
    MMI_IMAGE_ID_T              image_id;    /*!< image id*/  
    GUIANIM_DATA_INFO_T         anim_data;   /*!< ͼƬ����, ��set item contentʱ����, ����������*/ 
    GUIANIM_FILE_INFO_T         anim_path;   /*!< ͼƬ·��, ��set item contentʱ����, ����������*/ 
}GUISETLIST_DATA_U;

/*! @struct GUISETLIST_IMAGE_T
@brief ����ͼƬ���ݵ�����
*/
typedef struct
{
    uint16                  image_width;      /*!< ͼƬ��ʾ�Ŀ��*/
    uint16                  image_height;     /*!< ͼƬ��ʾ�ĸ߶�*/  
    GUISETLIST_DATA_TYPE_E  image_data_type;  /*!< ͼƬ��������*/ 
    GUISETLIST_DATA_U       image_data;       /*!< ͼƬ������Ϣ������*/ 
}GUISETLIST_IMAGE_T; 

/*! @struct GUISETLIST_BG_T
@brief ���屳��������Ϣ
*/
typedef struct  
{
    MMI_GUISETLIST_BG_TYPE_E bg_type;   /*!< ��������*/
    GUI_COLOR_T          color;			/*!< ������ɫ*/
    MMI_IMAGE_ID_T       image_id; 		/*!< ����ͼƬID*/
}GUISETLIST_BG_T;

/*! @struct guisetlist_item_t_tag
@brief   ����item������Ϣ
*/
typedef struct guisetlist_item_t_tag
{
    GUISETLIST_ITEM_STATE_T     item_state; /*!< item״̬*/
    MMI_STRING_T                str_info;   /*!< item�ַ�����Ϣ*/
    GUISETLIST_IMAGE_T          image_info; /*!< itemͼƬ��Ϣ*/
    uint32                      add_data;   /*!< ���ֶ������û���д,ϵͳʹ��*/ 
    BOOLEAN                     is_selected;/*!< ��ѡ��ʽ�£�item�Ƿ�ѡ��*/ 
}GUISETLIST_ITEM_INFO_T;

/*! @struct GUISETLIST_ITEM_T
@brief Item����
*/
typedef struct  
{
    uint32          item_total_num; /*!< ����*/
    MMI_LINK_NODE_T *item_head_ptr; /*!< ����*/
}GUISETLIST_ITEM_T;

/*! @struct GUISETLIST_INIT_DATA_T
@brief �ؼ��ĳ�ʼ�����ݡ����ؼ�����
*/
typedef struct 
{
    GUI_BOTH_RECT_T           both_rect; /*!< �ؼ�����������*/

}GUISETLIST_INIT_DATA_T; 

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief ����һ��Item
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_info_ptr [in] �ؼ�Item������Ϣ
@retval ���ص�ǰitem����������Append Item֮����ܵ�item��
@return ���ص�ǰitem������
@note ��item���������ĩβ������һ��item���ݣ�ͬʱitem������+1
*******************************************************************************/
PUBLIC uint32 CTRLSETLIST_AddItem(
                                 MMI_CTRL_ID_T ctrl_id, 
                                 const GUISETLIST_ITEM_INFO_T *item_info_ptr
                                 );

/***************************************************************************//*!
@brief ����text id����һ��Item
@author jinwang
@param ctrl_id [in] �ؼ�ID
@param text_id [in] text id
@retval ���ص�ǰitem����������Append Item֮����ܵ�item��
@return ���ص�ǰitem������
@note ��item���������ĩβ������һ��item���ݣ�ͬʱitem������+1
*******************************************************************************/
PUBLIC uint32 CTRLSETLIST_AddItemById(
                                 MMI_CTRL_ID_T ctrl_id, 
                                 MMI_TEXT_ID_T text_id
                                 );

/***************************************************************************//*!
@brief ����һ��Item
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_info_ptr [in] �ؼ�Item������Ϣ
@param item_index [in] ���������λ��
@retval TRUE �ɹ�
@retval FALSE ʧ��
@return ���سɹ���ʧ��
@note item_index ���ڵ�������ʱ��������������ĩβ
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_InsertItem(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     const GUISETLIST_ITEM_INFO_T *item_info_ptr,
                                     uint32 item_index
                                     );


/***************************************************************************//*!
@brief �޸�һ��Item
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_info_ptr [in] �ؼ�Item������Ϣ
@param item_index [in] ���������λ��
@retval TRUE �ɹ���FALSEʧ��
@return ���سɹ���ʧ��
@note ���޸�һ��item��ʱ��ʹ��item_info_ptr��ֵ�滻item_index��Ӧ��ֵ��
���item_index���ڵ����������򷵻�FALSE
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_ModifyItem(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     const GUISETLIST_ITEM_INFO_T *item_info_ptr,
                                     uint32 item_index
                                     );


/***************************************************************************//*!
@brief ����һ��Item
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_index [in] ������
@retval �ɹ���ʧ��
@return  ���سɹ���ʧ��
@note �������Ŵ��ڵ�������ʱ�������Ŵ�0��ʼ�����߿ؼ������ڣ��򷵻�FALSE
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_DeleteItem(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     uint32 item_index
                                     );

/***************************************************************************//*!
@brief ɾ�����нڵ�
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@retval �ɹ���ʧ��
@return  ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_DeleteAllItem(
                MMI_CTRL_ID_T ctrl_id
                         );

/***************************************************************************//*!
@brief Set the back ground color.
@author Lianxiang
@param ctrl_id [in] �ؼ�ID
@param bg_ptr [in] ����������Ϣ
@retval TRUE/FALSE
@return ���سɹ���ʧ��
@note ���ÿؼ��ı���,FALSE,���ô���,TRUE���óɹ������������Ϊ�գ���ؼ�������ʱ
����FALSE
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetBackGround(
                                        MMI_CTRL_ID_T ctrl_id, 
                                        GUI_BG_T *bg_ptr
                                        );


/***************************************************************************//*!
@brief  ���õ�ǰ�����ţ�����ǰ��ʾ��Item����
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_index [in] ������
@retval TRUE/FALSE
@return ������õ�item index��Ӧ�Ĳ�����,���ߴ����˵��ǲ��ɼ�,����FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetCurIndex(
                                      MMI_CTRL_ID_T ctrl_id, 
                                      uint32 item_index
                                      );


/***************************************************************************//*!
@brief ��ȡ��ǰ����,�����ǰ����Ϊ0,����0xffffffff
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@retval ���ص�ǰItem����
@return �������Ϊ0,����GUISETLIST_INVALID_ITEM_INDEX
@note 
*******************************************************************************/
PUBLIC uint32 CTRLSETLIST_GetCurIndex(
                                     MMI_CTRL_ID_T ctrl_id
                                     );


/***************************************************************************//*!
@brief ��̬�����ؼ�
@author xiaoqing.lu
@param win_handle [in] ���ھ�����򴰿�ID
@param ctrl_id [in] �ؼ�ID
@param rect_ptr [in] �ؼ�����
@retval TRUE/FALSE
@return ���ش����ɹ���ʧ��
@note ���ؼ��Ѿ����ڣ��������ڴ�ʧ��ʱ������FALSE
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_CreateCtrlDyn(
                                        MMI_HANDLE_T win_handle,
                                        MMI_CTRL_ID_T ctrl_id,
                                        GUI_BOTH_RECT_T *rect_ptr
                                        );


/***************************************************************************//*!
@brief  ���ÿؼ�����ʾ����
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param rect_ptr [in] ��ʾ����
@param is_update [in] �Ƿ�����ˢ��
@retval TRUE/FALSE
@return ���سɹ���ʧ��
@note ��ʾ��������������ؼ���rect������
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetDisplayRect(
                                         MMI_CTRL_ID_T ctrl_id,  //in:
                                         const GUI_RECT_T *rect_ptr,      //in:
                                         BOOLEAN          is_update       //in:
                                         );


/***************************************************************************//*!
@brief ��ÿؼ�����ʾ����
@author Jassmine
@param ctrl_id [in] �ؼ�ID
@param rect_ptr [in/out] �ؼ���ʾ����
@retval TRUE/FALSE
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_GetDisplayRect(
                                         MMI_CTRL_ID_T ctrl_id,  //in:
                                         GUI_RECT_T   *rect_ptr       //in/out:
                                         );


#ifdef TOUCH_PANEL_SUPPORT
/***************************************************************************//*!
@brief ���ÿؼ���tp���򣬼�tp���ʱ����Ӧ����
@author Jassmine 
@param ctrl_id [in] �ؼ�ID
@param rect_ptr [in] ����
@retval TRUE/FALSE
@return ���سɹ���ʧ��
@note ֻ��form�ؼ���ʹ�øýӿڣ�����Ӧ�ò�Ӧ��ʹ��
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetTpRect(
                                    MMI_CTRL_ID_T   ctrl_id,    //in:
                                    GUI_RECT_T      *rect_ptr   //in:
                                    );
#endif


/***************************************************************************//*!
@brief set control boder info
@author Jassmine
@param ctrl_id [in] �ؼ�ID
@param border_ptr [in] �߽���Ϣ
@retval TRUE/FALSE
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetBorder(
                                    MMI_CTRL_ID_T ctrl_id,  //in:
                                    GUI_BORDER_T  *border_ptr
                                    );


/***************************************************************************//*!
@brief set control boder info
@author Jassmine
@param ctrl_id [in] �ؼ�ID
@param font_ptr [in] ������Ϣ�����������ɫ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetFont(
                                  MMI_CTRL_ID_T ctrl_id,  //in:
                                  GUI_FONT_ALL_T  *font_ptr
                                  );


/***************************************************************************//*!
@brief set setting list is visible
@author Jassmine
@param ctrl_id [in] �ؼ�ID
@param is_visible [in] �ؼ��Ƿ�ɼ�
@param is_update [in] �Ƿ�����ˢ��
@retval true/false
@return ���سɹ���ʧ��
@note Ӧ��ʱע��ˢ�µ�ʱ��
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetVisible(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     BOOLEAN        is_visible,
                                     BOOLEAN        is_update
                                     );


/***************************************************************************//*!
@brief set setting list is circular handle
@author Jassmine
@param ctrl_id [in] �ؼ�ID
@param is_circular [in] �Ƿ�ѭ���л�
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetCircular(
                                      MMI_CTRL_ID_T     ctrl_id,
                                      BOOLEAN           is_circular
                                      );


/***************************************************************************//*!
@brief  set setting list left/right arrow icon
@author Jassmine
@param ctrl_id [in] �ؼ�ID
@param left_down [in] �Ƿ�ѭ���л�
@param left_up [in] �ؼ�ID
@param right_down [in] �Ƿ�ѭ���л�
@param right_up [in] �Ƿ�ѭ���л�
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetArrow(
                                   MMI_CTRL_ID_T     ctrl_id,
                                   MMI_IMAGE_ID_T   left_down,
                                   MMI_IMAGE_ID_T   left_up,
                                   MMI_IMAGE_ID_T   right_down,
                                   MMI_IMAGE_ID_T   right_up
                                   );


/***************************************************************************//*!
@brief һ���ص��������ú������ڴ�list����ʱ
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param list_ctrl_ptr [in] ��Ƕ��guilist�ؼ�ָ��
@param param [in] ��������
@param call_type [in] ִ�лص�������
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLSETLIST_CallBack(
                                MMI_CTRL_ID_T   ctrl_id,
                                IGUICTRL_T      *list_ctrl_ptr,
                                DPARAM          param,
                                MMI_SETLIST_CALL_TYPE_E call_type
                                );


/***************************************************************************//*!
@brief set title
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param str_ptr [in] �ַ���
@param is_set [in] ���û�ȡ������
@retval true/false
@return ���سɹ���ʧ��
@note if is_set = TRUE, set title; else if is_set = FALSE, release title
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetTitleInfo(
                                       MMI_CTRL_ID_T ctrl_id,
                                       MMI_STRING_T *str_ptr,
                                       BOOLEAN is_set
                                       );


/***************************************************************************//*!
@brief set title by text id
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param text_id [in] �ַ�������ԴID
@retval true/false
@return ���سɹ���ʧ��
@note �����ڲ�������text id��ת��Ϊ�ַ�����Ϣ��������
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetTitleTextId(
                                         MMI_CTRL_ID_T ctrl_id,
                                         MMI_TEXT_ID_T text_id
                                         );


/***************************************************************************//*!
@brief ���ÿؼ�״̬
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param aim_state [in] ״ֵ̬
@param is_set [in] ���û�ȡ������
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLSETLIST_SetCtrlState(
                                    MMI_CTRL_ID_T     ctrl_id, 
                                    uint32 aim_state,
                                    BOOLEAN is_set
                                    );


/***************************************************************************//*!
@brief set setting list string effect
@author Jassmine
@param ctrl_id [in] �ؼ�ID
@param effect [in] �ַ�����Ч
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetStrEffect(
                                       MMI_CTRL_ID_T    ctrl_id,
                                       GUIFONT_EFFECT_T effect
                                       );


/***************************************************************************//*!
@brief set setting list need arrow
@author Xiaoqing.Lu
@param ctrl_id [in] �ؼ�ID
@param is_need_arrow [in] �Ƿ���Ҫ��ͷ
@retval true/false
@return ���سɹ���ʧ��
@note default FALSE
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetNeedArrow(
                                       MMI_CTRL_ID_T     ctrl_id,
                                       BOOLEAN is_need_arrow
                                       );


/***************************************************************************//*!
@brief �������ֶ��뷽ʽ
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param font_align [in] ���뷽ʽ
@retval true/false
@return ���سɹ���ʧ��
@note default ALIGN_HVMIDDLE����������ʹ�ֱ���򶼾��У�
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetFontAlign(
                                       MMI_CTRL_ID_T ctrl_id,  //in:
                                       GUI_ALIGN_E font_align
                                       );

/***************************************************************************//*!
@brief ������ʾ���(��ѡ/��ѡ)
@author jin.wang
@param ctrl_id  [in] �ؼ�ID
@param type     [in] ���
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetStyleById(
    MMI_CTRL_ID_T ctrl_id, 
    GUISETLIST_TYPE_E type
);

/***************************************************************************//*!
@brief ��ȡitemѡ��״̬
@author jin.wang
@param ctrl_id      [in] �ؼ�ID
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_ResetAllItemState(MMI_CTRL_ID_T ctrl_id);

/***************************************************************************//*!
@brief ��ȡitemѡ��״̬
@author jin.wang
@param ctrl_id      [in] �ؼ�ID
@param item_index   [in] item��index
@return ����item״̬
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_GetItemState(
    MMI_CTRL_ID_T ctrl_id,
    uint32 item_index
);

/***************************************************************************//*!
@brief ����itemѡ��״̬
@author jin.wang
@param ctrl_id      [in] �ؼ�ID
@param item_index   [in] item��index
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetItemState(
    MMI_CTRL_ID_T ctrl_id,
    uint32 item_index,
    BOOLEAN item_state
);

/***************************************************************************//*!
@brief ����ѡ��item��ʾ�ַ�����
@author jin.wang
@param ctrl_id          [in] �ؼ�ID
@param str_ptr          [in] �ַ���Ϣ
@param is_need_update   [in] �Ƿ���Ҫˢ��
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetDispText(
    MMI_CTRL_ID_T ctrl_id, 
    MMI_STRING_T *str_ptr,
    BOOLEAN is_need_update
);

/***************************************************************************//*!
@brief ���ö�ѡģʽ��ѡ��item��text����
@author jin.wang
@param ctrl_id      [in] �ؼ�ID
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetDefaultText(MMI_CTRL_ID_T ctrl_id);

/***************************************************************************//*!
@brief ����ѡ��item��index
@author jin.wang
@param ctrl_id      [in] �ؼ�ID
@param idx_ptr      [in/out] ���ڴ���������������飬����Ϊ��
@param array_len    [in] ��ѡ�е��ܸ���
@return ѡ��item�ĸ���
@note �û������ȷ����������鵽��Ӧ�ö����ô�����ȵ��øýӿڻ��������Ȼ���ٷ����ڴ��ȡ���屻ѡ�����������
*******************************************************************************/
PUBLIC uint16 CTRLSETLIST_GetSelectItemIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16* idx_ptr,
    uint16 array_len
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
