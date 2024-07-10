/*****************************************************************************
** File Name:      mmi_mainmenu_data.h                                       *
** Author:                                                                   *
** Date:           10/10/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2012      xiaoqing.lu              Create
******************************************************************************/

#ifndef _MMI_MAINMENU_DATA_H_
#define _MMI_MAINMENU_DATA_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlmainmenu_export.h"
#include "mmi_link.h"
#include "mmitheme_mainmenu.h"

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
// main menu item data
typedef struct
{
    uint32 menu_id;               /*!< 菜单 ID */
    MMI_TEXT_ID_T  text_id;       /*!< 文本 */
    MMI_IMAGE_ID_T icon_id;       /*!< 图标 */
    MMI_IMAGE_ID_T active_img_id; /*!< 焦点状态下可能使用动态资源图片 */

    CTRLMAINMENU_ITEM_FUNC call_back; /*!< callback */
    uint32 user_data;

} MMI_MAINMENU_ITEM_T;


// for user to modify
typedef struct
{
    uint32 group_id;
    uint32 menu_id;

} MMIMAINMENU_ITEM_INFO_T;

// list_head_ptr->data = (uint32)(MMIMAINMENU_ITEM_INFO_T*)
typedef struct
{
    uint32 item_count;
    MMI_LINK_NODE_T *list_head_ptr;

} MMIMAINEMENU_ITEM_LIST_T;

// list_head_ptr->data = (uint32)(MMIMAINEMENU_ITEM_LIST_T*)
typedef struct
{
    uint32 page_count;
    MMI_LINK_NODE_T *list_head_ptr;

} MMIMAINEMENU_PAGE_LIST_T;


typedef struct  
{
    uint16 item_num; // 实际item数
    MMIMAINMENU_ITEM_INFO_T menu_item_info[MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM]; // item信息
}MMIMAINMENU_PAGE_ITEM_INFO_T; // item info of every page

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_SetItemText(
    uint32 menu_id,
    MMI_STRING_T *str_ptr
);

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_SetItemInvisiable(
    uint32 menu_id,
    BOOLEAN is_true
);

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_SetItemDisable(
    uint32 menu_id,
    BOOLEAN is_true
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_IsItemDisable(
    uint32 menu_id
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_IsItemInvisiable(
    uint32 menu_id
);

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_GetItemData(
    uint32 menu_id,
    uint32 group_id,
    CTRLMAINMENU_ITEM_T *item_ptr
);

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC MMIMAINEMENU_ITEM_LIST_T *MMIMAINMENU_GetItemList(void);

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_ClearItemList(void);

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_AddItemNode(
    MMI_LINK_NODE_T **list_head_ptr,
    uint32 node
);

/*****************************************************************************/
//  Description : free一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_FreeItemNode(
    MMI_LINK_NODE_T const *node_ptr
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_FreePageNode(
    MMI_LINK_NODE_T const *node_ptr
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_GetItem(
    MMIMAINEMENU_ITEM_LIST_T *item_list_ptr, // in
    uint16 index,  // in
    MMIMAINMENU_ITEM_INFO_T *item_ptr // in/out
);

/*****************************************************************************/
//  Description : read matrix menu item info
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_ReadAllMenuItemInfo(
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 *num_ptr
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_MMI_MAINMENU_DATA_H_

/*@}*/
