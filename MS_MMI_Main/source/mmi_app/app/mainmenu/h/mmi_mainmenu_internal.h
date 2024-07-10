/***************************************************************************
** File Name:      MySample_image.h                                        *
** Author:         hua.fang                                                *
** Date:           08/03/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _MMI_MAINMENU_INTERNAL_H_
#define _MMI_MAINMENU_INTERNAL_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "mmi_mainmenu_data.h"
#include "mmi_link.h"
#include "mmitheme_mainmenu.h"
#include "guimenu.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : 添加页信息
// Global resource dependence : 
//  Author:hua.fang
// Note:
/*****************************************************************************/
PUBLIC MMIMAINEMENU_ITEM_LIST_T* MMIMENU_AddOnePage(
    MMIMAINEMENU_PAGE_LIST_T    *page_list_ptr
    );

/*****************************************************************************/
// Description : 往页表里添加item
// Global resource dependence : 
//  Author:hua.fang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_AddItemInPage(
    MMIMAINEMENU_ITEM_LIST_T        *item_list_ptr,
    MMIMAINMENU_ITEM_INFO_T         *item_info_ptr,
    uint16                          item_num
    );

/*****************************************************************************/
// Description : 初始化链表
// Global resource dependence : 
//  Author:jin.wang
// Note: 既可以是初始化,也可用于销毁
/*****************************************************************************/
PUBLIC void MMIMENU_DestroyPageList(
                                   MMIMAINEMENU_PAGE_LIST_T    *page_list_ptr
                                   );

/*****************************************************************************/
// Description : 销毁item链表
// Global resource dependence : 
//  Author:jin.wang
// Note: 
/*****************************************************************************/
PUBLIC void MMIMENU_DestroyItemList(
    MMIMAINEMENU_ITEM_LIST_T    *item_list_ptr
    );

/*****************************************************************************/
// Description : 删除一页
// Global resource dependence : 
//  Author: jin.wang
//  RETRUN:  TRUE: init success!
// Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DeleteOnePage(
                                  MMIMAINEMENU_PAGE_LIST_T      *page_list_ptr,
                                  uint16                        page_index
                                  );

/*****************************************************************************/
// Description : search in list  
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T *MMIMENU_GetItemListNode(
                                           MMIMAINEMENU_PAGE_LIST_T *page_list_ptr, 
                                           uint16                   item_index
                                           );

/*****************************************************************************/
// Description : get item
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC MMIMAINEMENU_ITEM_LIST_T *MMIMENU_GetItemList(
                                                   MMIMAINEMENU_PAGE_LIST_T *page_list_ptr,
                                                   uint32                   page_index
                                                   );

/*****************************************************************************/
// Description : search in list  
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T *MMIMENU_GetItemInfoNode(
                                           MMIMAINEMENU_ITEM_LIST_T *page_item_info_ptr, 
                                           uint16                   item_index
                                           );

/*****************************************************************************/
// Description : get item info in list  
// Global resource dependence : 
//  Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC MMIMAINMENU_ITEM_INFO_T* MMIMENU_GetItemInfo(
                                                    MMIMAINEMENU_ITEM_LIST_T    *page_item_info_ptr, 
                                                    uint16                      item_index
                                                    );


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _MMI_MAINMENU_INTERNAL_H_
