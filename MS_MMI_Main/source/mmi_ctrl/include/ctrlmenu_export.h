/***************************************************************************
** File Name:      ctrlmenu_export.h                                       *
** Author:         Jassmine                                                *
** Date:           08/03/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011       Jassmine         Create                                   *
**												                                  						   *
****************************************************************************/
#ifndef _CTRLMENU_EXPORT_H_
#define _CTRLMENU_EXPORT_H_ 

/*---------------------------------------------------------------------------
                          Include Files                                    
 --------------------------------------------------------------------------*/
#include "guisoftkey.h"

/*---------------------------------------------------------------------------
                         Compiler Flag                                      *
 ---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------
                          MACRO DEFINITION                                 
 --------------------------------------------------------------------------*/
#define GUIMENU_DYNA_NODE_MAX_NUM       50  //dynamic menu max node number

/*---------------------------------------------------------------------------
                          TYPE AND STRUCT                                  
--------------------------------------------------------------------------*/
/*! @enum GUIMENU_STYLE_E
@brief �˵����
*/
typedef enum
{
    GUIMENU_STYLE_NULL        = 0,  /*!< �� */
    GUIMENU_STYLE_SECOND      = 1,  /*!< �����˵������ʾΪ�б���ʽ�������������˵�����Ķ����˵� */
    GUIMENU_STYLE_THIRD       = GUIMENU_STYLE_SECOND,  /*!< ��ȥ���˷������ʾͬGUIMENU_STYLE_SECOND */
    GUIMENU_STYLE_POPUP       = 5,  /*!< ����ʽ�˵�����ϵͳͳһ��������λ�úͿ�ȣ��û��������� */
    GUIMENU_STYLE_POPUP_RADIO = 6,  /*!< ����ʽ�˵����˵�����е�ѡ��radio��� */
    GUIMENU_STYLE_POPUP_CHECK = 7,  /*!< ����ʽ�˵����˵�����ж�ѡ��check��� */
    GUIMENU_STYLE_POPUP_AUTO  = 10, /*!< ����ʽ�˵�������������û����ã��һ�����ı�����Ӧ��� */
    GUIMENU_STYLE_POPUP_RECT  = 11, /*!< ����ʽ�˵���������Ϳ�ȿ����û����ã�ϵͳ����������Ӧ */
    GUIMENU_STYLE_OPTIONS     = 14, /*!< OptionsMenu */

    GUIMENU_STYLE_MAX
} GUIMENU_STYLE_E;

/*! @enum GUIMENU_EP_E
@brief ����GUIMENU_STYLE_POPUP_AUTO����£��ĸ��������
@details
��GUIMENU_STYLE_POPUP_AUTO����£��˵�������������û����ã�Ĭ������£�����˵�
�������Ͻ���Ϊ��㿪ʼ��ʾ������ĳЩ����£����������Ĳο�����Ƚ����ף����磬
��������ʱ����Ҫ��������Ϸ������˵�����ô�������½���Ϊ��㣬�˵����ϵ���
���������û��ͱȽ�������⡣
*/
typedef enum
{
    GUIMENU_EP_TOP_LEFT     = 0,    /*!< ���Ͻ�Ϊ��Ч��� */
    GUIMENU_EP_TOP_RIGHT    = 1,    /*!< ���Ͻ�Ϊ��Ч��� */
    GUIMENU_EP_BOTTOM_LEFT  = 2,    /*!< ���½�Ϊ��Ч��� */
    GUIMENU_EP_BOTTOM_RIGHT = 3     /*!< ���½�Ϊ��Ч��� */
} GUIMENU_EP_E;

/*! @struct GUIMENU_BUTTON_STYLE_E
@brief Button��ʾ����
*/
typedef enum
{
    GUIMENU_BUTTON_STYLE_NONE,                  /*!< ����ʾButton*/
    GUIMENU_BUTTON_STYLE_OK,                    /*!< ��ʾOK */
    GUIMENU_BUTTON_STYLE_CANCEL,                /*!< ��ʾCancel */
    GUIMENU_BUTTON_STYLE_OK_CANCEL,             /*!< ��ʾOK, Cancel����Button */
    GUIMENU_BUTTON_STYLE_MAX
} GUIMENU_BUTTON_STYLE_E;

/*! @struct GUIMENU_INIT_DATA_T
@brief �˵���ʼ������
*/
typedef struct
{
    BOOLEAN                 is_static;      /*!< �Ƿ�̬�˵� */
    uint16                  node_max_num;   /*!< �˵������ڵ�����ֻ���ڶ�̬�˵���Ĭ��Ϊ\link #GUIMENU_DYNA_NODE_MAX_NUM \endlink */
    GUI_BOTH_RECT_T         both_rect;      /*!< �������µ����� */
    GUIMENU_STYLE_E         menu_style;     /*!< �˵����ֻ�ڶ�̬�˵���Ч����̬�˵��ķ���ڶ���˵�����ʱ��ȷ�� */
    MMI_MENU_GROUP_ID_T     group_id;       /*!< �˵���id��ֻ���ھ�̬�˵� */
} GUIMENU_INIT_DATA_T;

/*! @struct  GUIMENU_TITLE_INFO_T
@brief title��Ϣ���ݽṹ
*/
typedef struct
{
    BOOLEAN                 is_static;      /*!< �Ƿ�̬�˵� */
    uint32                  node_id;        /*!< �˵��ڵ�ID @note is_staticΪFALSEʱ��Ч */
    MMI_MENU_GROUP_ID_T     group_id;       /*!< �˵���ID @note is_staticΪTRUEʱ��Ч */
    MMI_IMAGE_ID_T          title_icon_id;  /*!< title��ͼƬ */
    MMI_STRING_T            *title_ptr;     /*!< ָ��title��ָ�� @note ���ΪPNULL����ʾ���Դ�����ָ����ı�Ϊ�գ���ʾ���title�����ָ����Ч�ı�����ʾ����title */
    MMI_STRING_T            *sub_title_ptr; /*!< ָ��sub title��ָ�� @note ���ΪPNULL����ʾ���Դ�����ָ����ı�Ϊ�գ���ʾ���sub title�����ָ����Ч�ı�����ʾ����sub title */
} GUIMENU_TITLE_INFO_T;

/*! @struct  GUIMENU_ITEM_T
@brief �˵������ݽṹ
*/
typedef struct
{
    MMI_MENU_ID_T               menu_id;                            /*!< �˵���id */
    MMI_MENU_TIP_T              tip_id;                             /*!< ��ʱ���ã���TIP_NULL */
    MMI_TEXT_ID_T               button_id[GUISOFTKEY_BUTTON_NUM];  /*!< softkey����ʾ��Ϣ */
    MMI_TEXT_ID_T               text_str_id;                        /*!< �˵����ı�ID */
    MMI_IMAGE_ID_T              select_icon_id;                     /*!< ѡ�п��ͼ��ID */
    MMI_IMAGE_ID_T              unselect_icon_id;                   /*!< �˵�ͼ��ID */
    uint32                      active_image_id;                    /*!< ��ý���ʱ��ҳ�л�ͼ��ID������GUIMENU_STYLE_OPTION_PAGE����� */
    MMI_MENU_GROUP_ID_T         link_group_id;                      /*!< �Ӳ˵�����ID */
} GUIMENU_ITEM_T;

/*! @struct  GUIMENU_GROUP_T
@brief  �˵������ݽṹ
*/
typedef struct
{
    const GUIMENU_ITEM_T        *item_ptr;          /*!< ָ��˵���������׵�ַ */
    GUIMENU_STYLE_E             menu_style;         /*!< �˵���� */
    MMI_TEXT_ID_T               title_string_id;    /*!< �˵������ı�ID */
    MMI_IMAGE_ID_T              title_icon_id;      /*!< ����������ͼƬID */
    MMI_IMAGE_ID_T              title_numicon_id;   /*!< �ݲ�֧�� */
    MMI_IMAGE_ID_T              title_background;   /*!< �ݲ�֧�� */
    uint16                      item_count;         /*!< �˵���ĸ���������ͨ��\link #GUIMENU_ITEM_NUM \endlink���� */
} GUIMENU_GROUP_T;

// ��̬�˵���ͼ������
typedef enum
{
	DYNA_ITEM_ICON_ID,
	DYNA_ITEM_ICON_BUFFER,
}DYNA_ITEM_ICON_TYPE_E;

//��̬�˵���ͼ������ΪDYNA_ITEM_ICON_BUFFERʱ����Ҫ��д
typedef struct  
{
	uint8* data;
	uint16 height;
	uint16 width;
 } DYNA_ITEM_ICON_BUFFER_T;

/*! @struct  GUIMENU_DYNA_ITEM_T
@brief ��̬�˵������ݽṹ
*/
typedef struct
{
    BOOLEAN					is_grayed;      /*!< �Ƿ���� */
    MMI_STRING_T			*item_text_ptr; /*!< �˵����ı� */
	DYNA_ITEM_ICON_TYPE_E	item_type;  	/*��̬�˵���item icon����,Ĭ��ΪDYNA_ITEM_ICON_ID*/
    MMI_IMAGE_ID_T			select_icon_id; /*!< �˵���ͼ��ID��item_typeΪDYNA_ITEM_ICON_IDʱ������*/
	DYNA_ITEM_ICON_BUFFER_T icon_buffer;	/*!< �˵���ͼ����Ϣ��item_typeΪDYNA_ITEM_ICON_BUFFERʱ������*/
} GUIMENU_DYNA_ITEM_T;

/*! @struct  GUIMENU_CHECK_INFO_T
@brief CHECK״̬�Ĳ˵�����Ϣ
*/
typedef struct
{
    uint32                  node_id;        /*!< �˵��ڵ�ID @note ��̬�˵�ʱ��Ч */
    MMI_MENU_ID_T           menu_id;        /*!< �˵���ID @note ��̬�˵�ʱ��Ч */
    MMI_MENU_GROUP_ID_T     group_id;       /*!< �˵���ID @note ��̬�˵�ʱ��Ч */
} GUIMENU_CHECK_INFO_T;

/*! @struct  GUIMENU_BUTTON_INFO_T
@brief button��Ϣ���ݽṹ
*/
typedef struct
{
    BOOLEAN                 is_static;      /*!< �Ƿ�̬�˵� */
    uint32                  node_id;        /*!< �˵��ڵ�ID @note is_staticΪFALSEʱ��Ч */
    MMI_MENU_GROUP_ID_T     group_id;       /*!< �˵���ID @note is_staticΪTRUEʱ��Ч */
    GUIMENU_BUTTON_STYLE_E  button_style;   /*!< button style */
} GUIMENU_BUTTON_INFO_T;

/*! @struct  GUIMENU_POP_SEL_INFO_T
@brief ָ���˵������ݽṹ
*/
typedef struct
{
    BOOLEAN                 is_static;      /*!< �Ƿ�̬�˵� */
    uint32                  node_id;        /*!< �˵��ڵ�ID @note is_staticΪFALSEʱ��Ч */
    MMI_CTRL_ID_T           ctrl_id;        /*!< �˵��ؼ�ID */
    MMI_MENU_ID_T           menu_id;        /*!< �˵���ID @note is_staticΪTRUEʱ��Ч */
    MMI_MENU_GROUP_ID_T     group_id;       /*!< �˵���ID @note is_staticΪTRUEʱ��Ч */
} GUIMENU_POP_SEL_INFO_T;

/*---------------------------------------------------------------------------
                         FUNCTION DEFINITION                              
 --------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : return menu group id and menu id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_GetId(
                           MMI_HANDLE_T         ctrl_handle,    //in:
                           MMI_MENU_GROUP_ID_T  *group_id_ptr,  //in/out:
                           MMI_MENU_ID_T        *menu_id_ptr    //in/out:
                           );

/*****************************************************************************/
//  Description : set menu the first and selected item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetFirstAndSelItem(
                                        uint16          first_item_index,
                                        uint16          cur_item_index,
                                        MMI_HANDLE_T    ctrl_handle
                                        );

/*****************************************************************************/
//  Description :set menu title,for dynamic menu or volatile static menu title
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetTitle(
                              MMI_STRING_T      *title_text_ptr,//title text
                              MMI_CTRL_ID_T     ctrl_id         //control id
                              );

/***************************************************************************//*!
//  Description :����Title����չ����
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetTitleEx(
                                   MMI_CTRL_ID_T        ctrl_id,        // [in]
                                   GUIMENU_TITLE_INFO_T *title_info_ptr // [in]
                                   );

/***************************************************************************//*!
@brief ������̬�˵�
@author Jassmine
@param rect_ptr [in] ָ��ؼ���ʾ�����ָ�롣����ǵ���ʽ�˵��Ļ�����ֵ����ΪPNULL��
@param win_id [in] ����ID
@param ctrl_id [in] �ؼ�ID
@param menu_style [in] �ؼ����
@note �÷�����û��ָ���ڵ�����Ĭ��Ϊ\link #GUIMENU_DYNA_NODE_MAX_NUM \endlink��
�����Ҫ�Զ���ڵ�������ʹ��\link GUIMENU_CreatDynamicEx() \endlink��
*******************************************************************************/
PUBLIC void CTRLMENU_CreatDynamic(
                                  GUI_BOTH_RECT_T   *rect_ptr,  //may PNULL,pop-up no need rect
                                  MMI_WIN_ID_T      win_id,
                                  MMI_CTRL_ID_T     ctrl_id,
                                  GUIMENU_STYLE_E   menu_style  //menu style
                                  );

/***************************************************************************//*!
@brief ������̬�˵�
@author Jassmine
@param node_max_num [in] ���Ľڵ���
@param rect_ptr [in] ָ��ؼ���ʾ�����ָ�롣����ǵ���ʽ�˵��Ļ�����ֵ����ΪPNULL��
@param win_id [in] ����ID
@param ctrl_id [in] �ؼ�ID
@param menu_style [in] �ؼ����
@note
*******************************************************************************/
PUBLIC void CTRLMENU_CreatDynamicEx(
                                    uint16          node_max_num,   //in:
                                    GUI_BOTH_RECT_T *rect_ptr,      //may PNULL,pop-up no need rect
                                    MMI_WIN_ID_T    win_id,
                                    MMI_CTRL_ID_T   ctrl_id,
                                    GUIMENU_STYLE_E menu_style      //menu style
                                    );

/***************************************************************************//*!
@brief ��ȡ��ǰѡ�����ID
@author Jassmine
@param ctrl_id [in] �ؼ�ID
@return ѡ�����ID
@note �÷���ֻ�����ڶ�̬�˵�\n
��̬�˵���ȡ��ǰѡ����ο�\link GUIMENU_GetId() \endlink
*******************************************************************************/
PUBLIC uint32 CTRLMENU_GetCurNodeId(
                                    MMI_CTRL_ID_T   ctrl_id //control id
                                    );

/***************************************************************************//*!
@brief ����̬�˵��ϲ���ڵ�
@author Jassmine
@param nodex_index [in] �ڵ�����ֵ����0��ʼ
@param node_id [in] �û��Զ���Ľڵ�ID����1��ʼ
@param parent_node_id [in] ���ڵ�ID�������0��������ڵ�
@param node_item_ptr [in] ָ��˵�����Ϣ��ָ��
@param ctrl_id [in] �ؼ�ID
@note
-# �÷���ֻ�����ڶ�̬�˵�
-# nodex_index������ֵ���Ǹ��ݲ���֮ǰ���˵������в˵����˳�������㣬��0��ʼ
-# node_id�����1��ʼ����Ϊ0������ڵ�
*******************************************************************************/
PUBLIC void CTRLMENU_InsertNode(
                                uint16                  nodex_index,    //node index,from 0
                                uint32                  node_id,        //node id,from 1
                                uint32                  parent_node_id, //parent node id
                                GUIMENU_DYNA_ITEM_T     *node_item_ptr, //node item
                                MMI_CTRL_ID_T           ctrl_id         //control id
                                );

/***************************************************************************//*!
@brief �������ö�̬�˵������Ϣ
@author James
@param nodex_index [in] �ڵ�����ֵ����0��ʼ
@param node_id [in] �������õ��û��Զ���ڵ�ID����1��ʼ
@param parent_node_id [in] ���ڵ�ID�������0��������ڵ�
@param node_text_ptr [in] ָ��˵����ı���ָ��
@param ctrl_id [in] �ؼ�ID
@return
@note
-# �÷���ֻ�����ڶ�̬�˵�
-# nodex_index������ֵ���Ǹ��ݲ���֮ǰ���˵������в˵����˳�������㣬��0��ʼ
-# node_id�����1��ʼ����Ϊ0������ڵ�
*******************************************************************************/
PUBLIC void CTRLMENU_ReplaceNode(
                                 uint16         nodex_index,    //node index,from 0
                                 uint32         node_id,        //node id,from 1
                                 uint32         parent_node_id, //parent node id
                                 MMI_STRING_T   *node_text_ptr, //node text
                                 MMI_CTRL_ID_T  ctrl_id         //control id
                                 );

#if defined (MRAPP_SUPPORT)||defined (HERO_ENGINE_SUPPORT ) 
/*****************************************************************************/
//  Description : in dynamic menu, replace node, Я��icon���
//  Global resource dependence : 
//  Author: sky.qgp
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_ReplaceNodeEx(
                                   uint16               node_index,     //node index,from 0
                                   uint32               node_id,        //node id,from 1
                                   uint32               parent_node_id, //parent node id
                                   GUIMENU_DYNA_ITEM_T  *node_item_ptr, //node item
                                   MMI_CTRL_ID_T        ctrl_id         //control id
                                   );
#endif

/***************************************************************************//*!
@brief ���ö�̬�˵����Ƿ����
@author Jassmine
@param is_grayed [in] �Ƿ����
@param nodex_index [in] �ڵ�����ֵ����0��ʼ
@param parent_node_id [in] ���ڵ�ID�������0��������ڵ�
@param ctrl_id [in] �ؼ�ID
@return
@note
*******************************************************************************/
PUBLIC void CTRLMENU_SetNodeGrayed(
                                  BOOLEAN       is_grayed,      //set node grayed
                                  uint16        nodex_index,    //node index,from 0
                                  uint32        parent_node_id, //parent node id
                                  MMI_CTRL_ID_T ctrl_id         //control id
                                  );

/*****************************************************************************/
//  Description : in dynamic menu, set node grayed
//  Global resource dependence : 
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetNodeGrayedByNodeId(
                                           BOOLEAN          is_grayed,      //set node grayed
                                           uint32           node_id,                // [in]
                                           uint32           parent_node_id, //parent node id
                                           MMI_CTRL_ID_T    ctrl_id         //control id
                                           );

/***************************************************************************//*!
@brief ���ò˵����Ƿ�ɼ�
@author hua.fang
@param ctrl_id [in] �ؼ�ID
@param nodex_index [in] �ڵ�����ֵ����0��ʼ
@param parent_node_id [in] ���ڵ�ID�������0��������ڵ�
@param is_visible [in] �Ƿ�ɼ�
@return �Ƿ����óɹ�
@note �÷���ֻ�����ڶ�̬�˵�����̬�˵��ɵ���\link GUIMENU_SetItemVisible() \endlink
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetNodeVisible(
                                       MMI_CTRL_ID_T    ctrl_id,        // [in]
                                       uint16           nodex_index,    // [in] node index,from 0
                                       uint32           parent_node_id, // [in]
                                       BOOLEAN          is_visible      // [in]
                                       );

/***************************************************************************//*!
@brief  ����softkey����ʾ����
@author Jassmine
@param ctrl_id [in] �ؼ�ID
@param left_softkey_text_id [in] ������ı�ID
@param middle_softkey_text_id [in] ������ı�ID
@param right_softkey_text_id [in] ������ı�ID
@return
@note
*******************************************************************************/
PUBLIC void CTRLMENU_SetDynamicMenuSoftkey(
                                           MMI_CTRL_ID_T    ctrl_id,
                                           MMI_TEXT_ID_T    left_softkey_text_id,
                                           MMI_TEXT_ID_T    middle_softkey_text_id,
                                           MMI_TEXT_ID_T    right_softkey_text_id
                                           );

/***************************************************************************//*!
@brief ���ò˵�����ɫ
@author Jassmine
@param ctrl_id [in] �ؼ�ID
@param bg_color [in] ����ɫ
@return
@note
*******************************************************************************/
PUBLIC void CTRLMENU_SetBgColor(
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_COLOR_T		bg_color
                                );

/***************************************************************************//*!
@brief ���ò˵����ı���ɫ
@author Jassmine
@param ctrl_id [in] �ؼ�ID
@param font_color [in] �ı���ɫ
@return
@note
*******************************************************************************/
PUBLIC void CTRLMENU_SetFontColor(
                                  MMI_CTRL_ID_T ctrl_id,
                                  GUI_COLOR_T   font_color
                                  );

/***************************************************************************//*!
@brief ��ȡradio��check�����ѡ��״̬�Ĳ˵���
@author hua.fang
@param ctrl_id [in] �ؼ�ID
@param check_info_ptr [in] ѡ��״̬�Ĳ˵�����Ϣ����
@param array_len [in] ���鳤��
@return �ܹ���ѡ�и���
@note �û������ȷ����������鵽��Ӧ�ö����ô�����ȵ��øýӿڻ��������Ȼ���ٷ����ڴ��ȡ���屻ѡ����
*******************************************************************************/
PUBLIC uint16 CTRLMENU_GetCheckInfo(
                                   MMI_CTRL_ID_T        ctrl_id,
                                   GUIMENU_CHECK_INFO_T *check_info_ptr,
                                   uint16               array_len
                                   );

/***************************************************************************//*!
@brief ���ò˵�����ʾ����
@author Jassmine
@param ctrl_handle [in] �ؼ�ID
@param rect_ptr [in] ָ����ʾ�����ָ��
@return
@note
*******************************************************************************/
PUBLIC void CTRLMENU_SetRect(
                             MMI_HANDLE_T   ctrl_handle,
                             GUI_RECT_T     *rect_ptr
                             );

/***************************************************************************//*!
@brief ���ò˵����Ƿ����
@author hua.fang
@param ctrl_id [in] �ؼ�ID
@param group_id [in] �˵���ID
@param menu_id [in] �˵���ID
@param is_grayed [in] �Ƿ����
@return �Ƿ����óɹ�
@note �÷���ֻ�����ھ�̬�˵�
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetItemGrayed(
                                      MMI_CTRL_ID_T         ctrl_id,    // [in]
                                      MMI_MENU_GROUP_ID_T   group_id,   // [in]
                                      MMI_MENU_ID_T         menu_id,    // [in]
                                      BOOLEAN               is_grayed   // [in]
                                      );

/***************************************************************************//*!
@brief ���ò˵����Ƿ�ɼ�
@author hua.fang
@param ctrl_id [in] �ؼ�ID
@param group_id [in] �˵���ID
@param menu_id [in] �˵���ID
@param is_visible [in] �Ƿ�ɼ�
@return �Ƿ����óɹ�
@note �÷���ֻ�����ھ�̬�˵�����̬�˵��ɵ���\link GUIMENU_SetNodeVisible() \endlink
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetItemVisible(
                                       MMI_CTRL_ID_T        ctrl_id,    // [in]
                                       MMI_MENU_GROUP_ID_T  group_id,   // [in]
                                       MMI_MENU_ID_T        menu_id,    // [in]
                                       BOOLEAN              is_visible  // [in]
                                       );

/*****************************************************************************/
//  Description : set item text and image
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetStaticItem(
                                      MMI_CTRL_ID_T         ctrl_id,    // [in]
                                      MMI_MENU_GROUP_ID_T   group_id,   // [in]
                                      MMI_MENU_ID_T         menu_id,    // [in]
                                      MMI_STRING_T          *text_ptr,  // [in] PNULL��ʾ�������ı�
                                      MMI_IMAGE_ID_T        icon_id     // [in] 0����IMAGE_NULL��ʾ������ͼƬ
                                      );

/***************************************************************************//*!
@brief ��������Ӧ�˵��������
@author hua.fang
@param ctrl_id [in] �ؼ�ID
@param max_horz_width [in] �����µ������
@param max_vert_width [in] �����µ������
@return �Ƿ����óɹ�
@note �÷�������\link #GUIMENU_STYLE_POPUP_AUTO \endlink����²�������
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetMaxWidth(
                                    MMI_CTRL_ID_T   ctrl_id,        // [in]
                                    uint16          max_horz_width, // [in]
                                    uint16          max_vert_width  // [in]
                                    );

/***************************************************************************//*!
@brief ����\link #GUIMENU_STYLE_POPUP_AUTO \endlink����£��ĸ��������ֵ
@author hua.fang
@param ctrl_id [in] �ؼ�ID
@param effective_point [in] ָ����Ч��
@return �Ƿ����óɹ�
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetEffectivePoint(
                                          MMI_CTRL_ID_T     ctrl_id,          // [in]
                                          GUIMENU_EP_E      effective_point   // [in]
                                          );

/*****************************************************************************/
//  Description : get state
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetState( 
                                 MMI_CTRL_ID_T  ctrl_id,
                                 uint32         src_state
                                 );

/*****************************************************************************/
//  Description : set state
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetState( 
                                 MMI_CTRL_ID_T  ctrl_id,
                                 uint32         src_state,
                                 BOOLEAN        is_true
                                 );

/*****************************************************************************/
//  Description : set current selected item
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: �����Զ�����topֵ
/*****************************************************************************/
PUBLIC void CTRLMENU_SetSelectedItem(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     uint16         cur_item_index
                                     );

/*****************************************************************************/
//  Description : ����button����ʾ���
//  Global resource dependence : 
//  Author: hua.fang
//  Note:�÷���ֻ��GUIMENU_STYLE_POPUP_AUTO,GUIMENU_STYLE_POPUP_CHECK,GUIMENU_STYLE_POPUP_RADIO,GUIMENU_STYLE_POPUP_RECT��Ч
/*****************************************************************************/
PUBLIC void CTRLMENU_SetButtonStyle(
                                    MMI_CTRL_ID_T           ctrl_id,         // [in]
                                    GUIMENU_BUTTON_INFO_T   *button_info_ptr // [in]
                                    );

/*****************************************************************************/
//  Description : get node string
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_GetNodeStr(
                                   MMI_CTRL_ID_T    ctrl_id,        //in:
                                   uint16           node_index,     //in:
                                   MMI_STRING_T     *node_str_ptr   //in/out:
                                   );

/*****************************************************************************/
//  Description : is pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsPopMenuEx(
                                    MMI_HANDLE_T    ctrl_handle
                                    );

/***************************************************************************//*!
@brief ���ø���
@author hua.fang
@param ctrl_id [in] �ؼ�ID
@param item_height [in] �����˵��ĺ���
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetPopItemHeight(
                                         MMI_CTRL_ID_T  ctrl_id,    // [in]
                                         uint16         item_height // [in]
                                         );

/*****************************************************************************/
//  Description : set pop-up menu item status for radio or check pop-up menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetPopItemStatus(
                                      BOOLEAN                   is_selected,
                                      GUIMENU_POP_SEL_INFO_T    *selected_info_ptr
                                      );

/***************************************************************************//*!
@brief ���ö����Ӳ˵��ķ��
@author hua.fang
@param ctrl_id [in] �ؼ�ID
@param node_id [in] �ڵ�ID
@param style [in] �˵����
@return
@note
-# �÷���ֻ�����ڶ�̬�˵�
-# style��ȡֵ��ΧΪ{\link #GUIMENU_STYLE_POPUP \endlink,
   \link #GUIMENU_STYLE_POPUP_RADIO \endlink, \link #GUIMENU_STYLE_POPUP_CHECK \endlink}��
ԭ���ķ��Ҳ���������ϼ��֡�
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetSubMenuStyle(
                                        MMI_CTRL_ID_T       ctrl_id,    // [in]
                                        uint32              node_id,    // [in]
                                        GUIMENU_STYLE_E     style       // [in]
                                        );

/*****************************************************************************/
//  Description : is first display menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_IsFirstDisp(
                                    MMI_CTRL_ID_T   ctrl_id //in:
                                    );

/*****************************************************************************/
//  Description : set is first display menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetFirstDisp(
                                     MMI_CTRL_ID_T  ctrl_id,    //in:
                                     BOOLEAN        is_first    //in:
                                     );

/*****************************************************************************/
//  Description : get menu level
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLMENU_GetLevel(
                                MMI_CTRL_ID_T   ctrl_id //in:
                                );

/*****************************************************************************/
//  Description : get menu rect, display rect,not full screen
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T CTRLMENU_GetRect(
                                   MMI_CTRL_ID_T    ctrl_id //in:
                                   );
/*****************************************************************************/
//  Description : set menu font size
//  Global resource dependence : 
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetSecMenuFontSize(MMI_CTRL_ID_T ctrl_id,GUI_FONT_T font,GUI_FONT_T expand_font);
/*****************************************************************************/
//  Description : set menu item height
//  Global resource dependence : 
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLMENU_SetSecMenuItemHeight(MMI_CTRL_ID_T ctrl_id,uint16 item_height,uint16 expand_item_height);
/***************************************************************************//*!
@brief ����ĳ���˵����ı�����ɫ
@author longwei.qiao
@param ctrl_id [in] �ؼ�ID
@param group_id [in]
@param item_index [in] �˵��������ֵ
@param font_color [in] ��Ҫ��ʾ����ɫ
@return �Ƿ����óɹ�
@note �÷������ھ�̬�����˵�����²�������
*******************************************************************************/
PUBLIC BOOLEAN CTRLMENU_SetItemFontColor(
                                       MMI_CTRL_ID_T        ctrl_id,    // [in]
                                       MMI_MENU_GROUP_ID_T  group_id,   // [in]
                                       uint16               item_index,// [in]
                                       GUI_COLOR_T          font_color  // [in]
                                       );

/**--------------------------------------------------------------------------
                          Compiler Flag
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
