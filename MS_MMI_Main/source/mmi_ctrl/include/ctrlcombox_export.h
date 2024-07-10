/*****************************************************************************
** File Name:      ctrlcombox_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      xiyuan.ma & nan.ji              Creat
******************************************************************************/

#ifndef _CTRLCOMBOX_EXPORT_H_
#define _CTRLCOMBOX_EXPORT_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "ctrlbutton_export.h"
#include "ctrllist_export.h"
#include "ctrltextedit_export.h"
#include "ctrldropdownlist_export.h"
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
/*! @enum GUICOMBOX_DETAIL_STR_DATA_T
@brief ���ʲ����ؼ�ʱ�����������λ��
*/
typedef struct
{
    MMI_STRING_T  str;
    MMI_STRING_T  detail_str;
} CTRLCOMBOX_DETAIL_STR_DATA_T;

/*! @struct GUICOMBOX_TYPE_T
@brief ���ÿؼ�������
*/
typedef struct
{
    GUIEDIT_TYPE_E edit_type; /*!< ���ÿؼ�������*/

    uint32   editable_max_text_num;
    uint32   editable_max_listitem_num;

} CTRLCOMBOX_TEXT_TYPE_T;

/*! @enum GUICOMBOX_SHOW_TYPE_E
@brief �������б�ʱ��ʾ��ʽ
*/
typedef enum
{
    CTRLCOMBOX_SHOW_DOWNWARD,    /*!< ���´��б��*/
    CTRLCOMBOX_SHOW_UPWARD,	    /*!< ���ϴ��б��*/
    CTRLCOMBOX_SHOW_AUTOSET,     /*!< ���ݿؼ���λ������Ӧ*/
    CTRLCOMBOX_SHOW_MAX
} CTRLCOMBOX_SHOW_TYPE_E;

/*! @enum GUICOMBOX_INIT_DATA_T
@brief ��ʼ���Ĳ���
*/
typedef struct
{
    GUI_BOTH_RECT_T  both_rect;  /*!<����������*/
    uint16                      page_item_num; /*!< ÿҳ��ʾ���������������������ʾ����*/
    CTRLCOMBOX_SHOW_TYPE_E       expand_direction; /*!< �������б�ʱ��ʾ��ʽ*/
} CTRLCOMBOX_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Set Expand Direction
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:����list��չ������,������,����,�Զ����ַ�ʽ.(������ʱĬ���Զ�)
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetExpandDirection(
    MMI_CTRL_ID_T    ctrl_id,
    CTRLCOMBOX_SHOW_TYPE_E expand_direction
);

/*****************************************************************************/
// 	Description : Disable WebKey
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:�����Ƿ�֧��web key(�û����ô˽ӿ�ȥ����web key�Ƿ���)
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_DisableWebKey(
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN      is_web_key_open_list
);

/*****************************************************************************/
// 	Description : Disable Mid Softkey
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:�����Ƿ�֧��mid key(�û����ô˽ӿ�ȥ����mid key�Ƿ���)
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_DisableMidSoftkey(
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN      is_mid_softkey_open_list
);

/*****************************************************************************/
// 	Description : Set Has Icon
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:�����Ƿ���icon
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetHasIcon(
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN      has_icon
);

/*****************************************************************************/
// 	Description : Set Icon Id
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:����icon��id
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetIconId(
    MMI_CTRL_ID_T    ctrl_id,
    uint32      icon_id
);

/*****************************************************************************/
// 	Description : Set combox text
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:����edit������
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetText(
    MMI_CTRL_ID_T  ctrl_id,
    MMI_STRING_T	*str_ptr
);

/*****************************************************************************/
// 	Description : Get combox text
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:��ȡedit������
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_GetText(
    MMI_CTRL_ID_T    ctrl_id,
    MMI_STRING_T     *str_ptr    //in:/out
);

/*****************************************************************************/
// 	Description : Set Function Button Callback
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:����func�Ļص���button
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetFuncButtonCallBack(
    MMI_CTRL_ID_T               ctrl_id,
    GUIBUTTON_CALLBACK_FUNC_EXT func,
    void*                       user_data //
);
/*****************************************************************************/
// 	Description : Add Item Detail
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:���detail��ʽ��list����
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_AddItemDetail(
    MMI_CTRL_ID_T ctrl_id,
    const CTRLCOMBOX_DETAIL_STR_DATA_T *data_ptr//item info
);

/*****************************************************************************/
// 	Description : Add Item
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:���detail��ʽ������
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_AddItemDetailArray(
    MMI_CTRL_ID_T ctrl_id,
    const CTRLCOMBOX_DETAIL_STR_DATA_T *data_ptr,	// appointed Item
    uint16 array_size
);
/*****************************************************************************/
// 	Description : Add Item
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:���һ���ַ���
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_AddItem(
    MMI_CTRL_ID_T ctrl_id,
    //const CTRLLIST_ITEM_T*   item_ptr	// appointed Item
    const MMI_STRING_T *str_ptr //item info
);

/*****************************************************************************/
// 	Description : Add Item
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:���һ���ַ�������
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_AddItemArray(
    MMI_CTRL_ID_T ctrl_id,
    const MMI_STRING_T *str_ptr,	// appointed Item
    uint16 array_size
);

/*****************************************************************************/
// 	Description : Insert Item
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:��list�в���һ��item
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_InsertItem(MMI_CTRL_ID_T ctrl_id,
                                     const CTRLLIST_ITEM_T*   item_ptr,// appointed Item
                                     uint16 pos
                                    );
/*****************************************************************************/
// 	Description : Remove Item
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:ɾ��һ��item
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_RemoveItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 pos
);

/*****************************************************************************/
// 	Description : Remove All Items
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:ɾ�����е�items
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_RemoveAllItems(
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
// 	Description : Get Item Count
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:���item����Ŀ
/*****************************************************************************/
PUBLIC uint16 CTRLCOMBOX_GetItemCount(
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
// 	Description : Get Item
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:���ĳһ��item
/*****************************************************************************/
PUBLIC const CTRLLIST_ITEM_T* CTRLCOMBOX_GetItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 index
);

/*****************************************************************************/
// 	Description : Get Select Index
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:��ȡselect item��index
/*****************************************************************************/
PUBLIC uint16 CTRLCOMBOX_GetSelectIndex(
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
// 	Description : Get Select Index
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:����select item��index
/*****************************************************************************/
PUBLIC uint16 CTRLCOMBOX_SetSelectIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16 index
);

/*****************************************************************************/
// 	Description : Set Select Index
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:����edit������(���ڵ����������û����ô˽ӿڵ�ʱ��ȥ����edit�ؼ�,
//       ԭ����edit����ʱ��Ҫ���������û��ڴ���openʱ������,����ֻ�������ʱ�򴴽�edit)
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetEditType(
    MMI_CTRL_ID_T ctrl_id,
    const CTRLCOMBOX_TEXT_TYPE_T *edit_type_ptr
);

/***************************************************************************//*!
// 	Description : Get Select Index
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:���edit������
*******************************************************************************/
PUBLIC GUIEDIT_TYPE_E CTRLCOMBOX_GetEditType(
    MMI_CTRL_ID_T ctrl_id
);

/***************************************************************************//*!
// 	Description : Set List Item Max Num
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:����list�������Ŀ
*******************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetListItemMaxNum(
    MMI_CTRL_ID_T ctrl_id,
    uint16        max_item_num,
    BOOLEAN       is_dynamic_item_data
);

/***************************************************************************//*!
// 	Description : combox has icon
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:����icon��id
*******************************************************************************/
PUBLIC void CTRLCOMBOX_SetIcon(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN is_show_icon,
    MMI_IMAGE_ID_T img_id
);


/***************************************************************************//*!
// 	Description : combox has expand button
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:�����Ƿ���expand button
*******************************************************************************/
PUBLIC void CTRLCOMBOX_SetExpandButton(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN is_show_expand_button
);

/***************************************************************************//*!
// 	Description : combox has function button
//	Global resource dependence :
//  Author: nan.ji & xiyuan.ma
//	Note:�����Ƿ���func button
*******************************************************************************/
PUBLIC void CTRLCOMBOX_SetFuncButton(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN is_show_fucn_button
);

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:����combox������
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetRect(
    MMI_HANDLE_T ctrl_handle,
    const GUI_RECT_T* rect_ptr
);
/*****************************************************************************/
// 	Description : Set BoxList Space
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:����list��combox�ļ��
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetBoxListSpace(
    MMI_CTRL_ID_T    ctrl_id,
    uint16      box_list_space
);
/*****************************************************************************/
// 	Description : Set RightAligned
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:�����Ƿ��Ҷ���
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetRightAligned(
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN     is_right_aligned
);
/*****************************************************************************/
// 	Description : Set Fixed List
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:�����Ƿ��ǹ̶���ʾlist
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetFixedList(
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN     is_list_fixed
);
/*****************************************************************************/
//  Description : ���edit��type
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:��ӳɹ�����TRUE��ʧ�ܷ���FALSE
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLCOMBOX_GetInlineEditHandle(
    MMI_HANDLE_T  win_handle,    //window id
    MMI_CTRL_ID_T ctrl_id // control id
);

/*****************************************************************************/
//  Description : ���edit��type
//  Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//  Note:��ӳɹ�����TRUE��ʧ�ܷ���FALSE
/*****************************************************************************/
PUBLIC uint16 CTRLCOMBOX_GetTotalItemNum(
    MMI_CTRL_ID_T ctrl_id // control id
);
/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_OpenList(
    MMI_HANDLE_T  win_handle,    //window id
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
// 	Description : open dropdown list
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_CloseList(
    MMI_HANDLE_T  win_handle,    //window id
    MMI_CTRL_ID_T ctrl_id
);
/*****************************************************************************/
// 	Description : Get Edit String
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_GetEditString(
    MMI_CTRL_ID_T ctrl_id,
    MMI_STRING_T *str_ptr 
);


/*****************************************************************************/
// 	Description : Clear Edit All String
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_ClearEditAllStr(
    MMI_CTRL_ID_T    ctrl_id
);


/*****************************************************************************/
// 	Description : Get Edit List Item Number
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC uint16 CTRLCOMBOX_GetEditListItemNum(
    MMI_CTRL_ID_T    ctrl_id
);


/*****************************************************************************/
// 	Description : Get Edit Current sor Current Position
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC uint16 CTRLCOMBOX_GetEditCursorCurPos(
    MMI_CTRL_ID_T    ctrl_id
);


/*****************************************************************************/
// 	Description : Get Edit List Item String
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_GetEditListItemString(
    MMI_CTRL_ID_T ctrl_id,
    uint16 array_len, 
    GUIEDIT_LIST_ITEM_STR_T *item_array_ptr
);


/*****************************************************************************/
// 	Description : Is Edit TP Panel Visible
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_IsEditTPPanelVisible(
    MMI_CTRL_ID_T    ctrl_id
);


/*****************************************************************************/
// 	Description : Set Edit Softkey
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_SetEditSoftkey(
    MMI_CTRL_ID_T ctrl_id,
    uint16 index, 
    uint16 str_len, 
    MMI_TEXT_ID_T text_1, 
    MMI_TEXT_ID_T text_2, 
    MMITHEME_EDIT_SET_BUTTON_PFUNC setsoftkey
);

/*****************************************************************************/
// 	Description : Set Edit Default String By Id
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetEditDefaultStringById(
    MMI_CTRL_ID_T ctrl_id,
    MMI_TEXT_ID_T default_string_id
);


/*****************************************************************************/
// 	Description : Set Edit Font
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_SetEditFont(
    MMI_CTRL_ID_T ctrl_id,
    GUI_FONT_T font
);


/*****************************************************************************/
// 	Description : Add Edit List Item Array
//	Global resource dependence :
//  Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_AddEditListItemArray(
    MMI_CTRL_ID_T ctrl_id,
    GUIEDIT_LIST_ITEM_T *list_item_ptr, 
    uint16 list_item_count
);


/*****************************************************************************/
// 	Description : Is Edit Active
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_IsEditActive(
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
// 	Description : Is Edit Active
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetEditActive(
    MMI_HANDLE_T win_handle,
    MMI_CTRL_ID_T ctrl_id
);
/*****************************************************************************/
// 	Description : Set Edit Bg
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLCOMBOX_SetEditBg(
    MMI_CTRL_ID_T ctrl_id,
    GUI_BG_T *bg_ptr
);


/*****************************************************************************/
// 	Description : Get Edit List Item Array
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC void CTRLCOMBOX_GetEditListItemArray(
    MMI_CTRL_ID_T ctrl_id,
    uint16 array_len,
    GUIEDIT_LIST_ITEM_T *item_array_ptr //out
);

/*****************************************************************************/
//  Description : get list handle
//  Global resource dependence : 
//  Author: xiyuan.ma & nan.ji
//  Note:��ӳɹ�����TRUE��ʧ�ܷ���FALSE
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLCOMBOX_GetInlineListHandle(
    MMI_CTRL_ID_T ctrl_id // control id
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

/*@}*/
