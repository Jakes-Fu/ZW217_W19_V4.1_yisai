#ifndef _GUICOMBOX_H_
#define _GUICOMBOX_H_

/*---------------------------------------------------------------------------
                          Include Files
 --------------------------------------------------------------------------*/
#include "guictrl.h"
#include "guilistbox.h"
#include "guiedit.h"
#include "guibutton.h"
/*---------------------------------------------------------------------------
                         Compiler Flag
 --------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

    /*--------------------------------------------------------------------------
                              MACRO DEFINITION
     --------------------------------------------------------------------------*/
#define  GUICOMBOX_SetFuncButtonCallBack CTRLCOMBOX_SetFuncButtonCallBack

#define  GUICOMBOX_DisableWebKey  CTRLCOMBOX_DisableWebKey

#define  GUICOMBOX_DisableMidSoftkey  CTRLCOMBOX_DisableMidSoftkey

#define  GUICOMBOX_SetHasIcon  CTRLCOMBOX_SetHasIcon

#define  GUICOMBOX_SetIconId  CTRLCOMBOX_SetIconId

#define  GUICOMBOX_SetText  CTRLCOMBOX_SetText

#define  GUICOMBOX_GetText  CTRLCOMBOX_GetText

#define  GUICOMBOX_AddItem  CTRLCOMBOX_AddItem

#define  GUICOMBOX_AddItemArray  CTRLCOMBOX_AddItemArray

#define  GUICOMBOX_RemoveItem  CTRLCOMBOX_RemoveItem

#define  GUICOMBOX_RemoveAllItems  CTRLCOMBOX_RemoveAllItems

#define  GUICOMBOX_GetItemCount  CTRLCOMBOX_GetItemCount

#define  GUICOMBOX_GetSelectIndex  CTRLCOMBOX_GetSelectIndex

#define  GUICOMBOX_SetSelectIndex  CTRLCOMBOX_SetSelectIndex

#define  GUICOMBOX_GetEditType  CTRLCOMBOX_GetEditType

#define  GUICOMBOX_SetListItemMaxNum  CTRLCOMBOX_SetListItemMaxNum

#define  GUICOMBOX_SetIcon  CTRLCOMBOX_SetIcon

#define  GUICOMBOX_SetExpandButton  CTRLCOMBOX_SetExpandButton

#define  GUICOMBOX_SetFuncButton  CTRLCOMBOX_SetFuncButton

#define  GUICOMBOX_SetRect  CTRLCOMBOX_SetRect

#define  GUICOMBOX_SetBoxListSpace  CTRLCOMBOX_SetBoxListSpace

#define  GUICOMBOX_SetRightAligned   CTRLCOMBOX_SetRightAligned

#define  GUICOMBOX_SetFixedList  CTRLCOMBOX_SetFixedList

#define  GUICOMBOX_GetInlineEditHandle  CTRLCOMBOX_GetInlineEditHandle

#define  GUICOMBOX_GetTotalItemNum  CTRLCOMBOX_GetTotalItemNum

#define  GUICOMBOX_OpenList  CTRLCOMBOX_OpenList

#define  GUICOMBOX_CloseList  CTRLCOMBOX_CloseList

#define  GUICOMBOX_GetEditString  CTRLCOMBOX_GetEditString

#define  GUICOMBOX_ClearEditAllStr  CTRLCOMBOX_ClearEditAllStr

#define  GUICOMBOX_GetEditListItemNum  CTRLCOMBOX_GetEditListItemNum

#define  GUICOMBOX_GetEditCursorCurPos  CTRLCOMBOX_GetEditCursorCurPos

#define  GUICOMBOX_GetEditListItemString  CTRLCOMBOX_GetEditListItemString

#define  GUICOMBOX_IsEditTPPanelVisible  CTRLCOMBOX_IsEditTPPanelVisible

#define  GUICOMBOX_SetEditSoftkey  CTRLCOMBOX_SetEditSoftkey

#define  GUICOMBOX_SetEditDefaultStringById  CTRLCOMBOX_SetEditDefaultStringById

#define  GUICOMBOX_SetEditFont  CTRLCOMBOX_SetEditFont

#define  GUICOMBOX_AddEditListItemArray  CTRLCOMBOX_AddEditListItemArray

#define  GUICOMBOX_IsEditActive  CTRLCOMBOX_IsEditActive

#define  GUICOMBOX_SetEditActive  CTRLCOMBOX_SetEditActive

#define  GUICOMBOX_SetEditBg  CTRLCOMBOX_SetEditBg

#define  GUICOMBOX_GetEditListItemArray    CTRLCOMBOX_GetEditListItemArray

#define  GUICOMBOX_GetInlineListHandle    CTRLCOMBOX_GetInlineListHandle

/*---------------------------------------------------------------------------
                          TYPE AND STRUCT
 --------------------------------------------------------------------------*/
/*! @enum GUICOMBOX_SHOW_TYPE_E
@brief 打开下拉列表时显示方式
*/
typedef enum
{
    GUICOMBOX_SHOW_DOWNWARD,    /*!< 向下打开列表框*/
    GUICOMBOX_SHOW_UPWARD,	    /*!< 向上打开列表框*/
    GUICOMBOX_SHOW_AUTOSET,     /*!< 根据控件的位置自适应*/
    GUICOMBOX_SHOW_MAX
} GUICOMBOX_SHOW_TYPE_E;


/*! @enum GUICOMBOX_DETAIL_STR_DATA_T
@brief 触笔操作控件时，点击的区域位置
*/
typedef struct
{
    MMI_STRING_T  str;
    MMI_STRING_T  detail_str;
} GUICOMBOX_DETAIL_STR_DATA_T;

/*! @struct GUICOMBOX_TYPE_T
@brief 设置控件的类型
*/
typedef struct
{
    GUIEDIT_TYPE_E edit_type; /*!< 设置控件的类型*/

    uint32   editable_max_text_num;
    uint32   editable_max_listitem_num;

} GUICOMBOX_TEXT_TYPE_T;

/*! @enum GUICOMBOX_INIT_DATA_T
@brief 初始化的参数
*/
typedef struct
{
    GUI_BOTH_RECT_T           both_rect;  /*!<横竖屏区域*/
    uint16                    page_item_num; /*!< 每页显示最大数，即下拉后所能显示的数*/
    GUICOMBOX_SHOW_TYPE_E     expand_direction; /*!< 打开下拉列表时显示方式*/
} GUICOMBOX_INIT_DATA_T;
/*---------------------------------------------------------------------------
                          FUNCTION DEFINITION
 --------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : GUICOMBOX SetExpandDirection
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC  BOOLEAN GUICOMBOX_SetExpandDirection (
    MMI_CTRL_ID_T    ctrl_id,
    GUICOMBOX_SHOW_TYPE_E expand_direction
);

/*****************************************************************************/
// 	Description : GUICOMBOX AddItemDetail
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC  BOOLEAN GUICOMBOX_AddItemDetail (
    MMI_CTRL_ID_T ctrl_id,
    const GUICOMBOX_DETAIL_STR_DATA_T *data_ptr
);

/*****************************************************************************/
// 	Description : GUICOMBOX AddItemDetailArray
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC  BOOLEAN GUICOMBOX_AddItemDetailArray (
    MMI_CTRL_ID_T ctrl_id,
    const GUICOMBOX_DETAIL_STR_DATA_T *data_ptr,
    uint16 array_size
);

/*****************************************************************************/
// 	Description : GUICOMBOX InsertItem
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC  BOOLEAN GUICOMBOX_InsertItem (
    MMI_CTRL_ID_T ctrl_id,
    const GUILIST_ITEM_T*   item_ptr,
    uint16 pos
);

/*****************************************************************************/
// 	Description : GUICOMBOX GetItem
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC const GUILIST_ITEM_T* GUICOMBOX_GetItem
(
    MMI_CTRL_ID_T ctrl_id,
    uint16 index
);

/*****************************************************************************/
// 	Description : GUICOMBOX SetEditType
//	Global resource dependence :
//    Author: xiyuan.ma & nan.ji
//	Note:
/*****************************************************************************/
PUBLIC  BOOLEAN GUICOMBOX_SetEditType (
    MMI_CTRL_ID_T ctrl_id,
    const GUICOMBOX_TEXT_TYPE_T *edit_type_ptr
);

/*--------------------------------------------------------------------------
                          Compiler Flag
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

/*@}*/
