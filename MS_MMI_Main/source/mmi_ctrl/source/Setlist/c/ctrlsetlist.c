/*****************************************************************************
** File Name:      ctrlsetlist.c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "mmi_theme.h"
#include "mmk_msg.h"
#include "mmk_timer.h"
#include "mmitheme_setlist.h"
#include "guicommon.h"
#include "guires.h"
#include "guistring.h"
#include "guisetlist.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "ctrlsetlist.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define VISIBLE_ITEM_NUM_1  1
#define VISIBLE_ITEM_NUM_2  2
#define INVALID_SELECT_CNT  0xffffffff

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : get setlist control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL CTRLSETLIST_OBJ_T* GetSetListPtr(
                                       MMI_HANDLE_T ctrl_handle
                                       );

/*****************************************************************************/
// Description : 创建一个服务信息结点指针
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUISETLIST_ITEM_INFO_T *CreateItemInfo(
                                             const GUISETLIST_ITEM_INFO_T *item_info_ptr
                                             );

/*****************************************************************************/
// Description : 复制一个服务信息结点指针
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CopyItemInfo(
                           const GUISETLIST_ITEM_INFO_T *src_info_ptr,
                           GUISETLIST_ITEM_INFO_T *des_info_ptr
                           );

/*****************************************************************************/
// Description : 销毁一个服务信息结点指针
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DestroyItemInfo(
                           GUISETLIST_ITEM_INFO_T *item_info_ptr
                           );

/*****************************************************************************/
// Description : Free the struct of GUISETLIST_ITEM_INFO_T
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DestroyItemNode(
                           MMI_LINK_NODE_T const * node_ptr
                           );

/*****************************************************************************/
// Description : 初始化链表
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void InitItemInfo(
                        CTRLSETLIST_OBJ_T *setlist_ptr
                        );

/*****************************************************************************/
// Description : search in list  
// Global resource dependence : 
// Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *SearchItemByIndex(
                                         CTRLSETLIST_OBJ_T const *setlist_ptr, 
                                         uint32 item_index
                                         );

/*****************************************************************************/
// Description : search in list  
// Global resource dependence : 
// Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *SearchNextVisibleItem(
                                             CTRLSETLIST_OBJ_T *setlist_ptr, 
                                             uint32 start_index, // 起始搜索位置
                                             BOOLEAN is_cyc // 是否循环搜索
                                             );

/*****************************************************************************/
// Description : search in list  
// Global resource dependence : 
// Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *SearchPreVisibleItem(
                                            CTRLSETLIST_OBJ_T *setlist_ptr, 
                                            uint32 start_index, // 起始搜索位置
                                            BOOLEAN is_cyc // 是否循环搜索
                                            );

/*****************************************************************************/
// Description : search in list  
// Global resource dependence : 
// Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *SearchCurVisibleItem(
                                            CTRLSETLIST_OBJ_T *setlist_ptr, 
                                            uint32 start_index
                                            );
/*****************************************************************************/
// Description : 获取状态
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL __inline BOOLEAN GetState(
                                uint32 src_state, 
                                uint32 aim_state
                                );
/*****************************************************************************/
// Description : 设置状态
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL __inline void SetState(
                             uint32 *src_state_ptr, 
                             uint32 aim_state,
                             BOOLEAN is_set
                             );

/*****************************************************************************/
// Description : 删除一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN: 
// Note:    
/*****************************************************************************/
LOCAL BOOLEAN DeleteItem(
                         CTRLSETLIST_OBJ_T *setlist_ptr,
                         uint32 item_index
                         );

/*****************************************************************************/
// Description : 删除所有Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN: 
// Note:    
/*****************************************************************************/
LOCAL BOOLEAN DeleteAllItem(
                         CTRLSETLIST_OBJ_T *setlist_ptr
                         );
                         
/*****************************************************************************/
// Description : 增加一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN:  返回当前item的总数
// Note:    
/*****************************************************************************/
LOCAL uint32 AddItem(
                     CTRLSETLIST_OBJ_T *setlist_ptr,
                     const GUISETLIST_ITEM_INFO_T *item_info_ptr
                     );

/*****************************************************************************/
// Description : 插入一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN: 返回实际插入的索引号
// Note:    
/*****************************************************************************/
LOCAL uint32 InsertItem(
                        CTRLSETLIST_OBJ_T *setlist_ptr,
                        const GUISETLIST_ITEM_INFO_T *item_info_ptr,
                        uint32 item_index
                        );

/*****************************************************************************/
// Description : 修改一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN: 
// Note:    
/*****************************************************************************/
LOCAL BOOLEAN ModifyItem(
                         CTRLSETLIST_OBJ_T *setlist_ptr,
                         const GUISETLIST_ITEM_INFO_T *item_info_ptr,
                         uint32 item_index
                         );

/*****************************************************************************/
// Description : 画背景
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DrawSetlistBg(
                         CTRLSETLIST_OBJ_T *setlist_ptr
                         );

/*****************************************************************************/
// Description : 获取图片显示的区域
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetImageDispRect(
                                  const CTRLSETLIST_OBJ_T *setlist_ptr,
                                  const GUISETLIST_ITEM_INFO_T *item_info_ptr
                                  );

/*****************************************************************************/
// Description : 获取文本显示的区域
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetTextRect(
                             const CTRLSETLIST_OBJ_T *setlist_ptr,
                             const GUISETLIST_ITEM_INFO_T *item_info_ptr
                             );

/*****************************************************************************/
// Description : 获取箭头显示的区域
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetArrowRect(
                              const CTRLSETLIST_OBJ_T *setlist_ptr,
                              BOOLEAN is_left
                              );

/*****************************************************************************/
// Description : 调整控件的区域
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DrawSetlistFrame(
                            CTRLSETLIST_OBJ_T *setlist_ptr
                            );

/*****************************************************************************/
// Description : 画背景
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DrawSetlistArrow(
                            CTRLSETLIST_OBJ_T   *setlist_ptr,
                            BOOLEAN             is_left, // 是否左侧，TRUE左侧，FALSE右侧
                            BOOLEAN             is_up // 是否弹起，TRUE弹起，FALSE按下
                            );

/*****************************************************************************/
// Description : destroy item data
// Global resource dependence : 
// Author:xiaoqing.lu
// Note: //is destroy anim ctrl
/*****************************************************************************/
LOCAL BOOLEAN DestroySetlistItemData(
                                     CTRLSETLIST_OBJ_T *setlist_ptr,
                                     GUISETLIST_ITEM_INFO_T *item_info_ptr
                                     );

/*****************************************************************************/
// Description : 画Items
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:注意:只画当前一个item
/*****************************************************************************/
LOCAL void DrawItemImage(
                         CTRLSETLIST_OBJ_T *setlist_ptr,
                         GUISETLIST_ITEM_INFO_T *item_info_ptr
                         );

/*****************************************************************************/
// Description : 画Item 文本
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DrawItemText(
                        CTRLSETLIST_OBJ_T *setlist_ptr,
                        GUISETLIST_ITEM_INFO_T *item_info_ptr
                        );

/*****************************************************************************/
// Description : 获取多选模式下要显示的text
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL void SetMultiDispText(CTRLSETLIST_OBJ_T *setlist_ptr);

/*****************************************************************************/
// Description : 设置item状态
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL void SetItemState(
    CTRLSETLIST_OBJ_T *setlist_ctrl_ptr,
    uint32 item_index,
    BOOLEAN is_selected
);

/*****************************************************************************/
// Description : 获取item状态
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN GetItemState(
    CTRLSETLIST_OBJ_T *setlist_ctrl_ptr,
    uint32 item_index
);

/*****************************************************************************/
// Description : 获取多选模式下要显示的text的长度
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL uint16 GetMultiDisTextLen(CTRLSETLIST_OBJ_T *setlist_ptr);

/*****************************************************************************/
//  Description : 获取选中item的index
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSelectedItem(
    const CTRLSETLIST_OBJ_T *list_ptr,
    uint16 array_len,
    uint16 *idx_ptr //[in/out]:index array,may PNULL
)
;
/*****************************************************************************/
// Description : 画Items
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:注意:只画当前一个item
/*****************************************************************************/
LOCAL void DrawSetlistItem(
                           CTRLSETLIST_OBJ_T    *setlist_ptr
                           );


#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// Description : 处理触笔按下的消息
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetListTpDown(
                                       CTRLSETLIST_OBJ_T *setlist_ptr, 
                                       GUI_POINT_T *point_ptr
                                       );

/*****************************************************************************/
// Description : 处理触笔弹起的消息
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetListTpUp(
                                     CTRLSETLIST_OBJ_T  *setlist_ptr, 
                                     DPARAM             param
                                     );
#endif

/*****************************************************************************/
// Description : handle MSG_KEYDOWN_LEFT message.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetListLeftKeyDown(
                                            CTRLSETLIST_OBJ_T *setlist_ptr
                                            );

/*****************************************************************************/
// Description : handle MSG_KEYDOWN_RIGHT message.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetListRightKeyDown(
                                             CTRLSETLIST_OBJ_T *setlist_ptr
                                             );

/*****************************************************************************/
// Description : handle MSG_KEYDOWN_WEB message.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetListWebKeyDown(
                                           CTRLSETLIST_OBJ_T *setlist_ptr
                                           );

/*****************************************************************************/
// Description : 统计可见的item num
// Global resource dependence : 
// Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL uint32 CountVisibleItemNum(
                                 CTRLSETLIST_OBJ_T *setlist_ptr
                                 );

/*****************************************************************************/
// Description : 调整控件的区域
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T AdjustSetlistRect(
                                   const CTRLSETLIST_OBJ_T *setlist_ptr
                                   );

/*****************************************************************************/
// Description : construct
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN GUISETLIST_Construct(
                                   CTRLBASE_OBJ_T           *base_ctrl_ptr,
                                   CTRLSETLIST_INIT_PARAM_T *setlist_param_ptr
                                   );

/*****************************************************************************/
// Description : destruct
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN GUISETLIST_Destruct(
                                  CTRLBASE_OBJ_T *base_ctrl_ptr
                                  );

/*****************************************************************************/
// Description : 状态栏的消息处理
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E GUISETLIST_HandleMessage(
                                            CTRLBASE_OBJ_T        *ctrl_ptr, //控件的指针
                                            MMI_MESSAGE_ID_E  msg_id,   //事件类型
                                            DPARAM            param     //附带的处理参数
                                            );

/*****************************************************************************/
// Description : set control display rect
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDisplayRect(
                             CTRLBASE_OBJ_T       *base_ctrl_ptr,  //in:
                             const GUI_RECT_T *rect_ptr,      //in:
                             BOOLEAN          is_update       //in:
                             );

/*****************************************************************************/
// Description : get control display rect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayRect(
                             CTRLBASE_OBJ_T   *base_ctrl_ptr,  //in:
                             GUI_RECT_T   *rect_ptr       //in/out:
                             );

/*****************************************************************************/
// Description : set control boder info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SetBorder(
                        CTRLBASE_OBJ_T    *base_ctrl_ptr,
                        GUI_BORDER_T  *border_ptr
                        );

/*****************************************************************************/
// Description : 设置显示的字符串
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDispTextInfo(
    CTRLSETLIST_OBJ_T *setlist_ctrl_ptr,
    MMI_STRING_T *text_ptr
);

/*****************************************************************************/
// Description : Set the back ground color.
// Global resource dependence : 
// Author: 
// Note:设置控件的背景,FALSE,设置错误,TRUE设置成功
/*****************************************************************************/
LOCAL BOOLEAN SetBackGround(
                            CTRLBASE_OBJ_T  *base_ctrl_ptr, 
                            GUI_BG_T    *bg_ptr
                            );

/*****************************************************************************/
// Description : set control font info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFont(
                      CTRLBASE_OBJ_T      *base_ctrl_ptr,
                      GUI_FONT_ALL_T  *font_ptr
                      );

/*****************************************************************************/
// Description : get control font info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFont(
                      CTRLBASE_OBJ_T      *base_ctrl_ptr,
                      GUI_FONT_ALL_T  *font_ptr
                      );

/*****************************************************************************/
// Description : set set list is circular handle left/right key msg
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SetCircularHandleLeftRight(
                                         CTRLBASE_OBJ_T *ctrl_ptr,
                                         BOOLEAN    is_circular
                                         );

/*****************************************************************************/
// Description : get control height by width,include content,border etc.
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN GetSetListHeight(
                               CTRLBASE_OBJ_T     *base_ctrl_ptr,  //in:
                               uint16         width,          //in:
                               uint16         *height_ptr     //in/out:
                               );
/*****************************************************************************/
// Description : set setlist rect
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetListSetRect(
                             CTRLBASE_OBJ_T       *ctrl_ptr,
                             const CAF_RECT_T *rect_ptr
                             );


/*****************************************************************************/
// Description : open win
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void OpenSetlistDispWin(
                              CTRLSETLIST_OBJ_T *setlist_ptr
                              );

/*****************************************************************************/
// Description : release title
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: 
/*****************************************************************************/
LOCAL void ReleaseSetlistTitleInfo(
                                   CTRLSETLIST_OBJ_T *setlist_ptr
                                   );

/*****************************************************************************/
// Description : release multi text
// Global resource dependence : 
// Author: jin.wang
// Note: 
/*****************************************************************************/
LOCAL void ReleaseMultiTextInfo(CTRLSETLIST_OBJ_T *setlist_ptr);


/*****************************************************************************/
// Description : set title
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: if is_set = TRUE, set title; else if is_set = FALSE, release title
/*****************************************************************************/
LOCAL BOOLEAN SetThisCtrlTitleInfo(
                                   CTRLSETLIST_OBJ_T *setlist_ptr,
                                   MMI_STRING_T *str_ptr,
                                   BOOLEAN is_set
                                   );

/*****************************************************************************/
//  Description : init setlist class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetlistCtrlInitVtbl(
    CTRLSETLIST_VTBL_T        *setlist_vtbl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void SetlistCtrlPackInitParam (
    GUISETLIST_INIT_DATA_T     *setlist_init_ptr,   //in
    CTRLSETLIST_INIT_PARAM_T   *setlist_param_ptr   //out
);

/*****************************************************************************/
//  Description : get setlist pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLSETLIST_OBJ_T* GetSetlistPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN SetlistTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get setlist type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T SETLIST_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "setlist",                                         // control name
                       sizeof (CTRLSETLIST_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) GUISETLIST_Construct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) GUISETLIST_Destruct,      // control destructor function
                       sizeof (CTRLSETLIST_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) SetlistCtrlPackInitParam,     // convert init param
                       sizeof (CTRLSETLIST_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) SetlistCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init setlist class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetlistCtrlInitVtbl (
    CTRLSETLIST_VTBL_T        *setlist_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) setlist_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = GUISETLIST_HandleMessage;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->SetRect   = SetListSetRect;
    
    base_ctrl_vtbl_ptr->SetBorder = SetBorder;
    base_ctrl_vtbl_ptr->SetBg     = SetBackGround;
    base_ctrl_vtbl_ptr->SetFont   = SetFont;
    base_ctrl_vtbl_ptr->GetFont   = GetFont;
    
	base_ctrl_vtbl_ptr->SetLeftRightLoop = SetCircularHandleLeftRight;
    base_ctrl_vtbl_ptr->GetHeightByWidth = GetSetListHeight;
    base_ctrl_vtbl_ptr->GetDisplayRect = GetDisplayRect;
    base_ctrl_vtbl_ptr->SetDisplayRect = SetDisplayRect;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
/*****************************************************************************/
LOCAL void SetlistCtrlPackInitParam (
    GUISETLIST_INIT_DATA_T     *setlist_init_ptr,   //in
    CTRLSETLIST_INIT_PARAM_T   *setlist_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) setlist_param_ptr;

    if (PNULL == setlist_init_ptr || PNULL == setlist_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = setlist_init_ptr->both_rect;
}

/*****************************************************************************/
//  Description : get setlist pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLSETLIST_OBJ_T* GetSetlistPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (SetlistTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLSETLIST_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN SetlistTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_SETLIST_TYPE);
}

/*****************************************************************************/
// Description : 获取状态
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL __inline BOOLEAN GetState(
                                uint32 src_state, 
                                uint32 aim_state
                                )
{
    if ((src_state&aim_state) != 0x0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// Description : 设置状态
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL __inline void SetState(
                             uint32 *src_state_ptr, 
                             uint32 aim_state,
                             BOOLEAN is_set
                             )
{
    if (is_set)
    {
        *src_state_ptr |= aim_state;
    }
    else
    {
        *src_state_ptr &= ~aim_state;
    }

    return;
}

/*****************************************************************************/
// Description : statusbar type of
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:该函数进行运行时类型判别
/*****************************************************************************/
LOCAL __inline BOOLEAN TypeOfSetList(
                                     CTRLBASE_OBJ_T* ctrl_ptr
                                     )
{
    BOOLEAN result = FALSE;
    
    if (PNULL != ctrl_ptr)
    {
        result = (BOOLEAN)( SPRD_GUI_SETLIST_ID == IGUICTRL_GetCtrlGuid((IGUICTRL_T*)ctrl_ptr ) );
    }

    return result;
}

/*****************************************************************************/
// Description : get setlist control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL CTRLSETLIST_OBJ_T* GetSetListPtr(
                                       MMI_HANDLE_T ctrl_handle
                                       )
{
    CTRLBASE_OBJ_T* ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr( ctrl_handle );
    
    // 如果为空，或者类型不同，则返回PNULL
    if (PNULL == ctrl_ptr || !TypeOfSetList((CTRLBASE_OBJ_T*)ctrl_ptr))
    {
        ctrl_ptr = PNULL;/*lint !e522*/
    }

    return (CTRLSETLIST_OBJ_T*)ctrl_ptr;
}


/*****************************************************************************/
// Description : 设置item状态
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL void SetItemState(
    CTRLSETLIST_OBJ_T *setlist_ctrl_ptr,
    uint32 item_index,
    BOOLEAN is_selected
)
{
    MMI_LINK_NODE_T *node_ptr = PNULL;
    GUISETLIST_ITEM_INFO_T *item_info_ptr = PNULL;

    if (PNULL == setlist_ctrl_ptr)
    {
        return;
    }
    
    node_ptr = SearchItemByIndex(setlist_ctrl_ptr, item_index);

    if (PNULL == node_ptr)
    {
        return;
    }
    
    item_info_ptr = (GUISETLIST_ITEM_INFO_T*)node_ptr->data;
    
    if (PNULL == item_info_ptr)
    {
        return;
    }

    if (item_info_ptr->is_selected == is_selected)
    {
        return;
    }

    item_info_ptr->is_selected = is_selected;

    setlist_ctrl_ptr->selected_cnt += (is_selected ? 1 : INVALID_SELECT_CNT);

    return;
}

/*****************************************************************************/
// Description : 获取item状态
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN GetItemState(
    CTRLSETLIST_OBJ_T *setlist_ctrl_ptr,
    uint32 item_index
)
{
    MMI_LINK_NODE_T *node_ptr = PNULL;
    GUISETLIST_ITEM_INFO_T *item_info_ptr = PNULL;

    if (PNULL == setlist_ctrl_ptr)
    {
        return FALSE;
    }
    
    node_ptr = SearchItemByIndex(setlist_ctrl_ptr, item_index);

    if (PNULL == node_ptr)
    {
        return FALSE;
    }
    
    item_info_ptr = (GUISETLIST_ITEM_INFO_T*)node_ptr->data;
    
    if (PNULL == item_info_ptr)
    {
        return FALSE;
    }
   
    return item_info_ptr->is_selected;   
}
/*****************************************************************************/
// Description : 创建一个服务信息结点指针
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUISETLIST_ITEM_INFO_T *CreateItemInfo(
                                             const GUISETLIST_ITEM_INFO_T *item_info_ptr
                                             )
{
    GUISETLIST_ITEM_INFO_T *new_item_info_ptr = PNULL;

    if (item_info_ptr != PNULL)
    {
        new_item_info_ptr = SCI_ALLOCA(sizeof(GUISETLIST_ITEM_INFO_T));
        if (PNULL != new_item_info_ptr)
        {
            SCI_MEMSET(new_item_info_ptr, 0, sizeof(GUISETLIST_ITEM_INFO_T));

            // 保证输入参数一定是GUISETLIST_ITEM_INFO_T类型,否则这样cp可能会内存覆盖
            SCI_MEMCPY(new_item_info_ptr, item_info_ptr, sizeof(GUISETLIST_ITEM_INFO_T));

            if (PNULL != item_info_ptr->str_info.wstr_ptr)
            {
                new_item_info_ptr->str_info.wstr_ptr = SCI_ALLOC_APP(sizeof(wchar)*item_info_ptr->str_info.wstr_len);

                SCI_MEMSET(new_item_info_ptr->str_info.wstr_ptr, 0, (sizeof(wchar)*item_info_ptr->str_info.wstr_len));

                SCI_MEMCPY(new_item_info_ptr->str_info.wstr_ptr, item_info_ptr->str_info.wstr_ptr,
                    (sizeof(uint16)*item_info_ptr->str_info.wstr_len));

                new_item_info_ptr->str_info.wstr_len = item_info_ptr->str_info.wstr_len;
            }
        }
    }

    return new_item_info_ptr;
}

/*****************************************************************************/
// Description : 复制一个服务信息结点指针
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN CopyItemInfo(
                           const GUISETLIST_ITEM_INFO_T *src_info_ptr,
                           GUISETLIST_ITEM_INFO_T *des_info_ptr
                           )
{
    BOOLEAN result = FALSE;

    if (PNULL != src_info_ptr && PNULL != des_info_ptr)
    {
        result = TRUE;
        
        if (PNULL != src_info_ptr->str_info.wstr_ptr)
        {
            if (PNULL != des_info_ptr->str_info.wstr_ptr)
            {
                SCI_FREE(des_info_ptr->str_info.wstr_ptr);
            }

            des_info_ptr->str_info.wstr_ptr = SCI_ALLOC_APP(sizeof(wchar)*src_info_ptr->str_info.wstr_len);
            
            SCI_MEMSET(des_info_ptr->str_info.wstr_ptr, 0, (sizeof(wchar)*src_info_ptr->str_info.wstr_len));
            
            SCI_MEMCPY(des_info_ptr->str_info.wstr_ptr, src_info_ptr->str_info.wstr_ptr,
                (sizeof(uint16)*src_info_ptr->str_info.wstr_len));
            
            des_info_ptr->str_info.wstr_len = src_info_ptr->str_info.wstr_len;
        }

        des_info_ptr->add_data = src_info_ptr->add_data;
        des_info_ptr->image_info = src_info_ptr->image_info;
        des_info_ptr->item_state = src_info_ptr->item_state;
    }

    return result;
}

/*****************************************************************************/
// Description : 销毁一个服务信息结点指针
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DestroyItemInfo(
                           GUISETLIST_ITEM_INFO_T *item_info_ptr
                           )
{
    if (item_info_ptr != PNULL)
    {
        if (PNULL != item_info_ptr->str_info.wstr_ptr)
        {
            SCI_FREE(item_info_ptr->str_info.wstr_ptr);
        }
        SCI_FREE(item_info_ptr);
    }

    return;
}

/*****************************************************************************/
// Description : Free the struct of GUISETLIST_ITEM_INFO_T
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DestroyItemNode(
                           MMI_LINK_NODE_T const * node_ptr
                           )
{
    GUISETLIST_ITEM_INFO_T *item_info_ptr = PNULL;

    if (node_ptr == PNULL)
    {
        return;
    }
    item_info_ptr = (GUISETLIST_ITEM_INFO_T*)node_ptr->data;

    DestroyItemInfo(item_info_ptr);

    return;
}

/*****************************************************************************/
// Description : 初始化链表
// Global resource dependence : 
// Author:xiaoqing.lu
// Note: 既可以是初始化,也可是销毁
/*****************************************************************************/
LOCAL void InitItemInfo(
                        CTRLSETLIST_OBJ_T *setlist_ptr
                        )
{
    if(PNULL != setlist_ptr)
    {
        if (PNULL != setlist_ptr->item_info.item_head_ptr)
        {
            MMILINK_DestroyLink(setlist_ptr->item_info.item_head_ptr, DestroyItemNode);
        }
        
        setlist_ptr->item_info.item_head_ptr = PNULL;
        setlist_ptr->item_info.item_total_num = 0;
        
        setlist_ptr->cur_item_index = 0;
    }
    
    return;
}

/*****************************************************************************/
// Description : 统计可见的item num
// Global resource dependence : 
// Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL uint32 CountVisibleItemNum(
                                 CTRLSETLIST_OBJ_T *setlist_ptr
                                 )
{
    uint32  item_num = 0;
    MMI_LINK_NODE_T *tmp_node_ptr = PNULL;
    MMI_LINK_NODE_T *tmp_head_ptr = PNULL;
    GUISETLIST_ITEM_INFO_T *item_info_ptr = PNULL;

    if (PNULL != setlist_ptr)
    {
        tmp_head_ptr = tmp_node_ptr = setlist_ptr->item_info.item_head_ptr;
        
        if (PNULL != tmp_node_ptr)
        {
            do
            {
                item_info_ptr = (GUISETLIST_ITEM_INFO_T *)tmp_node_ptr->data;
                
                if (PNULL!= item_info_ptr 
                    && !GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_INVISIBLE))
                {
                    item_num++;
                }
                
                // 循环链表
                tmp_node_ptr = tmp_node_ptr->next_ptr;
                
            }while (tmp_node_ptr != tmp_head_ptr);
        }
    }

    return item_num;    
}

/*****************************************************************************/
// Description : search in list  
// Global resource dependence : 
// Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *SearchItemByIndex(
                                         CTRLSETLIST_OBJ_T const *setlist_ptr, 
                                         uint32 item_index
                                         )
{
    BOOLEAN is_find = FALSE;
    uint32  i = 0;
    MMI_LINK_NODE_T *node_ptr = PNULL;
    MMI_LINK_NODE_T *tmp_head_ptr = PNULL;

    if (PNULL != setlist_ptr)
    {
        tmp_head_ptr = node_ptr = setlist_ptr->item_info.item_head_ptr;

        if (PNULL != node_ptr)
        {
            if (0 == item_index)
            {
                node_ptr = tmp_head_ptr;
                is_find = TRUE;
            }
            else
            {
                for (i = 0; i < setlist_ptr->item_info.item_total_num; i++)
                {
                    // 当回到起始点的时候，肯定是没找到
                    if (i == item_index && node_ptr != tmp_head_ptr) 
                    {
                        is_find = TRUE;
                        break;
                    }
                    // 指向下一个指针
                    node_ptr = node_ptr->next_ptr;
                }
            }
        }
    }

    if (!is_find)
    {
        // 如果没找到，初始化为PNULL
        node_ptr = PNULL;
    }

    return node_ptr;    
}

/*****************************************************************************/
// Description : search in list  
// Global resource dependence : 
// Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *SearchNextVisibleItem(
                                             CTRLSETLIST_OBJ_T *setlist_ptr, 
                                             uint32 start_index, // 起始搜索位置
                                             BOOLEAN is_cyc // 是否循环搜索
                                             )
{
    uint32          cur_item_index  = 0;
    BOOLEAN         is_find         = FALSE;
    MMI_LINK_NODE_T *tmp_node_ptr   = PNULL;
    MMI_LINK_NODE_T *tmp_start_ptr  = PNULL;
    MMI_LINK_NODE_T *tmp_head_ptr   = PNULL;
    GUISETLIST_ITEM_INFO_T *item_info_ptr = PNULL;

    if (PNULL != setlist_ptr)
    {
        // 记录原始的cur置
        cur_item_index = setlist_ptr->cur_item_index;

        // 查到起始位置指针
        tmp_start_ptr = SearchItemByIndex(setlist_ptr, start_index);

        // 检查是否存在
        if (PNULL != tmp_start_ptr)
        {
            // 记录起始位置指针，用于循环
            tmp_node_ptr = tmp_start_ptr->next_ptr;
            
            // 记录搜索的终点
            if (is_cyc) // 如果循环，连标的head在start点
            {
                tmp_head_ptr = tmp_start_ptr;
            }
            else 
            {
                // 如果不循环，链表的head在原始链表的起始处
                tmp_head_ptr = setlist_ptr->item_info.item_head_ptr;
            }
            
            // 开始查找
            if (PNULL != tmp_node_ptr)
            {
                while (tmp_node_ptr != tmp_head_ptr)
                {
                    item_info_ptr = (GUISETLIST_ITEM_INFO_T *)tmp_node_ptr->data;

                    // 计算当前的索引号
                    setlist_ptr->cur_item_index++;
                    setlist_ptr->cur_item_index %= setlist_ptr->item_info.item_total_num;

                    if (!GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_INVISIBLE) )
                    {
                        is_find = TRUE;
                        break;
                    }
                    tmp_node_ptr = tmp_node_ptr->next_ptr;
                }
            }
        }

        if (!is_find)
        {
            // 如果没找到，初始化为PNULL
            tmp_node_ptr = PNULL;

            // 没找到的时候当前索引号不变
            setlist_ptr->cur_item_index = cur_item_index;
        }
    }

    return tmp_node_ptr;    
}

/*****************************************************************************/
// Description : search in list  
// Global resource dependence : 
// Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *SearchCurVisibleItem(
                                            CTRLSETLIST_OBJ_T *setlist_ptr, 
                                            uint32 start_index
                                            )
{
    uint32          cur_item_index  = 0;
    BOOLEAN         is_find         = FALSE;
    MMI_LINK_NODE_T *tmp_node_ptr   = PNULL;
    MMI_LINK_NODE_T *tmp_start_ptr  = PNULL;
    MMI_LINK_NODE_T *tmp_head_ptr   = PNULL;
    GUISETLIST_ITEM_INFO_T *item_info_ptr = PNULL;

    if (PNULL != setlist_ptr)
    {
        // 记录原始的cur置
        cur_item_index = setlist_ptr->cur_item_index;

        // 查到起始位置指针
        tmp_start_ptr = SearchItemByIndex(setlist_ptr, start_index);

        // 检查是否存在
        if (PNULL != tmp_start_ptr)
        {
            // 记录起始位置指针，用于循环
            tmp_head_ptr = tmp_node_ptr = tmp_start_ptr;
            
            // 开始查找
            do
            {
                item_info_ptr = (GUISETLIST_ITEM_INFO_T *)tmp_node_ptr->data;
                
                if (PNULL!= item_info_ptr 
                    && !GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_INVISIBLE))
                {
                    is_find = TRUE;
                    break;
                }
                
                // 计算当前的索引号
                setlist_ptr->cur_item_index++;
                setlist_ptr->cur_item_index %= setlist_ptr->item_info.item_total_num;
                
                tmp_node_ptr = tmp_node_ptr->next_ptr;
                
            }while (tmp_node_ptr != tmp_head_ptr);
        }

        if (!is_find)
        {
            // 如果没找到，初始化为PNULL
            tmp_node_ptr = PNULL;

            // 没找到的时候当前索引号不变
            setlist_ptr->cur_item_index = cur_item_index;
        }
    }

    return tmp_node_ptr;    
}

/*****************************************************************************/
// Description : search in list  
// Global resource dependence : 
// Author:Xiaoqing.Lu
// Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T *SearchPreVisibleItem(
                                            CTRLSETLIST_OBJ_T *setlist_ptr, 
                                            uint32 start_index, // 起始搜索位置
                                            BOOLEAN is_cyc // 是否循环搜索
                                            )
{
    uint32          cur_item_index  = 0;
    BOOLEAN         is_find         = FALSE;
    MMI_LINK_NODE_T *tmp_node_ptr   = PNULL;
    MMI_LINK_NODE_T *tmp_start_ptr  = PNULL;
    MMI_LINK_NODE_T *tmp_end_ptr   = PNULL;
    GUISETLIST_ITEM_INFO_T *item_info_ptr = PNULL;

    if (PNULL != setlist_ptr)
    {
        // 记录原始的cur置
        cur_item_index = setlist_ptr->cur_item_index;

        // 查到起始位置指针
        tmp_start_ptr = SearchItemByIndex(setlist_ptr, start_index);

        // 检查是否存在
        if (PNULL != tmp_start_ptr)
        {
            // 记录起始位置指针，用于循环
            tmp_node_ptr = tmp_start_ptr->prev_ptr;
            
            // 记录搜索的终点
            if (is_cyc) // 如果循环，连标的head在start点
            {
                tmp_end_ptr = tmp_start_ptr;
            }
            else 
            {
                // 如果不循环，链表的结束位置在原始链表的head的前一个指针
                tmp_end_ptr = setlist_ptr->item_info.item_head_ptr->prev_ptr;
            }
            
            // 开始查找
            if (PNULL != tmp_node_ptr)
            {
                while (tmp_node_ptr != tmp_end_ptr)
                {
                    item_info_ptr = (GUISETLIST_ITEM_INFO_T *)tmp_node_ptr->data;

                    // 计算当前的索引号
                    if (setlist_ptr->item_info.item_total_num > setlist_ptr->cur_item_index 
                        && setlist_ptr->cur_item_index > 0)
                    {
                        setlist_ptr->cur_item_index--;
                    }
                    else if (setlist_ptr->cur_item_index == 0 )
                    {
                        setlist_ptr->cur_item_index = setlist_ptr->item_info.item_total_num - 1;
                    }
                    else
                    {
                        setlist_ptr->cur_item_index = 0;
                    }

                    if (!GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_INVISIBLE) )
                    {
                        is_find = TRUE;
                        break;
                    }
                    tmp_node_ptr = tmp_node_ptr->prev_ptr;
                }
            }
        }

        if (!is_find)
        {
            // 如果没找到，初始化为PNULL
            tmp_node_ptr = PNULL;

            // 没找到的时候当前索引号不变
            setlist_ptr->cur_item_index = cur_item_index;
        }
    }

    return tmp_node_ptr;    
}

/*****************************************************************************/
// Description : 删除一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN: 
// Note:    
/*****************************************************************************/
LOCAL BOOLEAN DeleteItem(
                         CTRLSETLIST_OBJ_T *setlist_ptr,
                         uint32 item_index
                         )
{
    BOOLEAN result = FALSE;
    MMI_LINK_NODE_T *node_ptr = PNULL;

    if((PNULL != setlist_ptr) && (TypeOfSetList((CTRLBASE_OBJ_T*)setlist_ptr)))
    {
        // 查找一个节点
        node_ptr = SearchItemByIndex(setlist_ptr, item_index);
        
        if (PNULL != node_ptr && 0 != setlist_ptr->item_info.item_total_num)
        {
            // 删除一个节点
            setlist_ptr->item_info.item_head_ptr = MMILINK_DestroyNode(node_ptr, setlist_ptr->item_info.item_head_ptr, DestroyItemNode);
            setlist_ptr->item_info.item_total_num--;
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
// Description : 删除所有Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN: 
// Note:    
/*****************************************************************************/
LOCAL BOOLEAN DeleteAllItem(
                         CTRLSETLIST_OBJ_T *setlist_ptr
                         )
{
    BOOLEAN result = FALSE;

    if((PNULL != setlist_ptr) && (TypeOfSetList((CTRLBASE_OBJ_T*)setlist_ptr)))
    {
        if(PNULL != setlist_ptr->item_info.item_head_ptr)
        {
            MMILINK_DestroyLink(setlist_ptr->item_info.item_head_ptr, DestroyItemNode);
            setlist_ptr->item_info.item_head_ptr = PNULL;
            setlist_ptr->item_info.item_total_num = 0;
        }
    }

    return result;
}

/*****************************************************************************/
// Description : 修改一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN: 
// Note:    
/*****************************************************************************/
LOCAL BOOLEAN ModifyItem(
                         CTRLSETLIST_OBJ_T *setlist_ptr,
                         const GUISETLIST_ITEM_INFO_T *item_info_ptr,
                         uint32 item_index
                         )
{
    BOOLEAN result = FALSE;
    MMI_LINK_NODE_T *node_ptr = PNULL;
    GUISETLIST_ITEM_INFO_T *new_item_info_ptr = PNULL;

    if ((PNULL != setlist_ptr) && (TypeOfSetList((CTRLBASE_OBJ_T*)setlist_ptr)))
    {
        // 查找一个节点
        node_ptr = SearchItemByIndex(setlist_ptr, item_index);
        
        if (PNULL != node_ptr && PNULL != node_ptr->data)
        {
            new_item_info_ptr = (GUISETLIST_ITEM_INFO_T*)node_ptr->data;
            
            result = CopyItemInfo(item_info_ptr, new_item_info_ptr);
        }
    }

    return result;
}

/*****************************************************************************/
// Description : 插入一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN: 返回实际插入的索引号
// Note:    
/*****************************************************************************/
LOCAL uint32 InsertItem(
                        CTRLSETLIST_OBJ_T *setlist_ptr,
                        const GUISETLIST_ITEM_INFO_T *item_info_ptr,
                        uint32 item_index
                        )
{
    uint32 result_index = 0;
    MMI_LINK_NODE_T *node_ptr = PNULL;
    GUISETLIST_ITEM_INFO_T *new_item_info_ptr = PNULL;

    if ((PNULL == setlist_ptr) || (!TypeOfSetList((CTRLBASE_OBJ_T*)setlist_ptr)))
    {
        //SCI_TRACE_LOW:"InsertItem error ctrl type!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_1281_112_2_18_3_21_19_292,(uint8*)"");
        return result_index;
    }
    
    // 查找一个节点
    node_ptr = SearchItemByIndex(setlist_ptr, item_index);
    // 创建一个服务信息节点,这里申请了内存
    new_item_info_ptr = CreateItemInfo(item_info_ptr);
    // 加在第一个节点
    if (PNULL == setlist_ptr->item_info.item_head_ptr)
    {
        setlist_ptr->item_info.item_head_ptr = MMILINK_CreateHead((uint32)new_item_info_ptr);
        setlist_ptr->item_info.item_total_num = 1;
        result_index = 0;
    }
    else
    {
        // 创建一个节点
        MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)new_item_info_ptr);
        if (PNULL == node_ptr) // 没有找到,则加载在链尾部
        {
            MMILINK_AddNodeBeforeBaseNode(setlist_ptr->item_info.item_head_ptr, tmp_node_ptr);
            result_index = setlist_ptr->item_info.item_total_num; // 尾部位置
        }
        else if (node_ptr == setlist_ptr->item_info.item_head_ptr) // 插入到头部
        {
            MMILINK_AddNodeBeforeBaseNode(setlist_ptr->item_info.item_head_ptr, tmp_node_ptr);
            setlist_ptr->item_info.item_head_ptr = setlist_ptr->item_info.item_head_ptr->prev_ptr; // 修改头指针
            result_index = 0; // 头部位置
        }
        else
        {
            MMILINK_AddNodeBeforeBaseNode(node_ptr, tmp_node_ptr); // 插入到tmp_ptr.prev_ptr的位置
            result_index = item_index; // 中间位置
        }
        setlist_ptr->item_info.item_total_num++;
    }

    return result_index;
}

/*****************************************************************************/
// Description : 增加一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN:  返回当前item的总数
// Note:    
/*****************************************************************************/
LOCAL uint32 AddItem(
                     CTRLSETLIST_OBJ_T *setlist_ptr,
                     const GUISETLIST_ITEM_INFO_T *item_info_ptr
                     )
{
    GUISETLIST_ITEM_INFO_T *new_item_info_ptr = PNULL;

    // 参数检查
    if(PNULL == setlist_ptr || !(TypeOfSetList((CTRLBASE_OBJ_T*)setlist_ptr)))
    {
        //SCI_TRACE_LOW:"AddItem error ctrl type!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_1337_112_2_18_3_21_20_293,(uint8*)"");
        return 0;
    }
    
    // 创建一个服务信息节点,这里申请了内存
    new_item_info_ptr = CreateItemInfo(item_info_ptr);
    // 参数检查
    if (PNULL != new_item_info_ptr 
        && setlist_ptr->item_info.item_total_num < setlist_ptr->item_max_num)
    {

        if (PNULL == setlist_ptr->item_info.item_head_ptr)
        {
            setlist_ptr->item_info.item_head_ptr = MMILINK_CreateHead((uint32)new_item_info_ptr);
        }
        else
        {
            MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)new_item_info_ptr);
            MMILINK_AddNodeBeforeBaseNode(setlist_ptr->item_info.item_head_ptr, tmp_node_ptr);
        }

        setlist_ptr->item_info.item_total_num++;
    }
    else // 如果没有加入列表项,这里是放内存
    {
        DestroyItemInfo(new_item_info_ptr);
        new_item_info_ptr = PNULL;
    }

    return setlist_ptr->item_info.item_total_num;
}

/*****************************************************************************/
// Description : construct
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN GUISETLIST_Construct(
                                   CTRLBASE_OBJ_T           *base_ctrl_ptr,
                                   CTRLSETLIST_INIT_PARAM_T *setlist_param_ptr
                                   )
{
    CTRLSETLIST_OBJ_T      *setlist_ptr = (CTRLSETLIST_OBJ_T *)base_ctrl_ptr;
    
    if( PNULL == base_ctrl_ptr || PNULL == setlist_param_ptr )
    {
        return FALSE;
    }
    
    setlist_ptr->is_valid = TRUE; // 默认情况下控件有效
    setlist_ptr->is_need_arrow = FALSE; // 默认情况下控件需要左右箭头

    // 设置控件父窗口ID
    setlist_ptr->win_id = MMK_GetWinHandleByCtrl( base_ctrl_ptr->handle );

    // 初始化默认风格
    MMITHEME_GetSetlistStyle(&setlist_ptr->default_style);

    if (GUISETLIST_TYPE_CHECKLIST == setlist_ptr->list_type)
    {
        //下左对齐
        setlist_ptr->default_style.align = ALIGN_LVMIDDLE;
    }
    
    // 初始化默认最大数
    setlist_ptr->item_max_num = MMITHEME_GetSetMaxItemNum();

    setlist_ptr->cur_item_index = 0; // 初始化当前item索引号

    base_ctrl_ptr->lcd_dev_info = *(MMITHEME_GetDefaultLcdDev()); // lcd信息

    setlist_ptr->ctrl_state |= GUISETLIST_STATE_OPEN_POPWIN|GUISETLIST_STATE_ISCYC;

    // 初始化item信息
    setlist_ptr->item_info.item_head_ptr = PNULL;
    setlist_ptr->item_info.item_total_num = 0;

    setlist_ptr->is_disptxt_auto = TRUE;

    return TRUE;
}

/*****************************************************************************/
// Description : destruct
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN GUISETLIST_Destruct(
                                  CTRLBASE_OBJ_T *base_ctrl_ptr
                                  )
{
    CTRLSETLIST_OBJ_T *setlist_ptr = (CTRLSETLIST_OBJ_T *)base_ctrl_ptr;

    // 销毁item信息
    InitItemInfo(setlist_ptr);
    
    ReleaseSetlistTitleInfo(setlist_ptr);

    ReleaseMultiTextInfo(setlist_ptr);
    
    return TRUE;
}

/*****************************************************************************/
// Description : set setlist rect
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetListSetRect(
                             CTRLBASE_OBJ_T       *ctrl_ptr,
                             const CAF_RECT_T *rect_ptr
                             )
{
    BOOLEAN             result = FALSE;
    CTRLSETLIST_OBJ_T     *setlist_ptr = PNULL;

    setlist_ptr = (CTRLSETLIST_OBJ_T *)ctrl_ptr;

    if (!GUI_EqualRect(ctrl_ptr->rect, *rect_ptr))
    {
        ctrl_ptr->rect = *rect_ptr;

        ctrl_ptr->display_rect = *rect_ptr;

        GUI_SetVOrHRect( ctrl_ptr->handle, (GUI_RECT_T *)rect_ptr,&(ctrl_ptr->both_rect));

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : 画背景
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DrawSetlistBg(
                         CTRLSETLIST_OBJ_T *setlist_ptr
                         )
{
    GUI_RECT_T          image_rect = {0};
    GUI_RECT_T          cross_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T *    ctrl_ptr   = (CTRLBASE_OBJ_T *)setlist_ptr;     
    
	if(PNULL != setlist_ptr
        && TypeOfSetList((CTRLBASE_OBJ_T*)setlist_ptr)
        && GUI_IntersectRect(&cross_rect, ctrl_ptr->display_rect, ctrl_ptr->rect))
    {
        if (MMITHEME_CheckImageID(setlist_ptr->default_style.bg_info.img_id))
        {
            // 计算背景图片显示的区域
            image_rect.bottom = image_rect.top + (cross_rect.bottom - cross_rect.top);
            image_rect.right = image_rect.left + (cross_rect.right - cross_rect.left);
        }

        //set bg display info
        bg_display.is_transparent = FALSE;
        bg_display.rect           = ctrl_ptr->rect;
        bg_display.display_rect   = cross_rect;
        bg_display.img_rect       = image_rect;
        bg_display.ctrl_handle    = ctrl_ptr->handle;
        bg_display.win_handle     = setlist_ptr->win_id;

        GUI_DisplayBg(&setlist_ptr->default_style.bg_info,
            &bg_display,
            &(ctrl_ptr->lcd_dev_info));
    }
}

/*****************************************************************************/
// Description : 获取图片显示的区域
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetImageDispRect(
                                  const CTRLSETLIST_OBJ_T *setlist_ptr,
                                  const GUISETLIST_ITEM_INFO_T *item_info_ptr
                                  )
{
    BOOLEAN    is_disp_image = TRUE;
    GUI_RECT_T image_rect = {0};
    GUI_RECT_T ctrl_rect = {0};

    if(PNULL == setlist_ptr || PNULL == item_info_ptr)
    {
        //SCI_TRACE_LOW:"GetImageDispRect error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_1523_112_2_18_3_21_20_294,(uint8*)"");
        return image_rect;
    }

    ctrl_rect = AdjustSetlistRect(setlist_ptr);

    switch(setlist_ptr->default_style.layout)
    {
    case GUISETLIST_LAYOUT_IMAGE_MIDDLE: // 只有图片,图片显示在中间,上下居中
        if (setlist_ptr->is_need_arrow)
        {
            GUI_RECT_T arrow_rect = {0};

            arrow_rect = GetArrowRect(setlist_ptr, TRUE);
            image_rect.left = arrow_rect.right + 1;
            arrow_rect = GetArrowRect(setlist_ptr, FALSE);
            image_rect.right = arrow_rect.left - 1;

        }
        else
        {
            image_rect.left = ctrl_rect.left + 1;
            image_rect.right = ctrl_rect.right - 1;
        }
        // 重新调整区域
        if (image_rect.right - image_rect.left + 1 > item_info_ptr->image_info.image_width)
        {
            image_rect.left = (image_rect.right + image_rect.left + 1 - item_info_ptr->image_info.image_width)/2;
            image_rect.right = image_rect.left + item_info_ptr->image_info.image_height - 1;
        }
        break;
    
    case GUISETLIST_LAYOUT_IMAGE_LEFT:   // 只有图片,图片显示在左侧,上下居中
    case GUISETLIST_LAYOUT_IMAGE_TEXT:
        if (setlist_ptr->is_need_arrow)
        {
            GUI_RECT_T arrow_rect = {0};

            arrow_rect = GetArrowRect(setlist_ptr, TRUE);

            image_rect.left = arrow_rect.right + 1;
        }
        else
        {
            image_rect.left = ctrl_rect.left + 1;
        }
        image_rect.right = MIN(ctrl_rect.right, image_rect.left + item_info_ptr->image_info.image_width - 1);
        break;

    case GUISETLIST_LAYOUT_IMAGE_RIGHT:  // 只有图片,图片显示在右侧,上下居中
    case GUISETLIST_LAYOUT_TEXT_IMAGE:
        if (setlist_ptr->is_need_arrow)
        {
            GUI_RECT_T arrow_rect = {0};

            arrow_rect = GetArrowRect(setlist_ptr, FALSE);

            image_rect.right = arrow_rect.right - 1;
        }
        else
        {
            image_rect.right = ctrl_rect.right - 1;
        }
        image_rect.left = MAX(ctrl_rect.left, image_rect.right - item_info_ptr->image_info.image_width + 1);
        break;
        
    default :
        is_disp_image = FALSE;
        //SCI_TRACE_LOW:"GetImageDispRect: not surport image!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_1591_112_2_18_3_21_20_295,(uint8*)"");
        break;
    }
    
    if (is_disp_image)
    {
        if (ctrl_rect.bottom - ctrl_rect.top - 1 > item_info_ptr->image_info.image_height)
        {
            image_rect.top = (ctrl_rect.bottom + ctrl_rect.top + 1 - item_info_ptr->image_info.image_height)/2;
            image_rect.bottom = image_rect.top + item_info_ptr->image_info.image_height - 1;
        }
        else
        {
            image_rect.top = ctrl_rect.top;
            image_rect.bottom = ctrl_rect.bottom;
        }
    }

    return image_rect;
}

/*****************************************************************************/
// Description : 获取文本显示的区域
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetTextRect(
                             const CTRLSETLIST_OBJ_T *setlist_ptr,
                             const GUISETLIST_ITEM_INFO_T *item_info_ptr
                             )
{
    GUI_RECT_T text_rect = {0};
    GUI_RECT_T image_rect = {0};
    GUI_RECT_T arrow_rect = {0};

    if (PNULL == setlist_ptr || PNULL == item_info_ptr)
    {
        //SCI_TRACE_LOW:"GetTextRect error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_1627_112_2_18_3_21_20_296,(uint8*)"");
        return text_rect;
    }

    text_rect = AdjustSetlistRect(setlist_ptr);
    
    if (setlist_ptr->is_need_arrow) // 需要箭头时，如果不需要箭头，则直接返回text rect
    {
        // 即显示图片，又显示文本的情况
        if (GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_TEXT)
            && GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_IMAGE))
        { 
            switch(setlist_ptr->default_style.layout)
            {
            case GUISETLIST_LAYOUT_IMAGE_TEXT:
                image_rect = GetImageDispRect(setlist_ptr, item_info_ptr);
                text_rect.left = image_rect.right + 1;
                
                if (setlist_ptr->is_need_arrow)
                {
                    arrow_rect = GetArrowRect(setlist_ptr, FALSE);
                    text_rect.right = arrow_rect.left - 1;
                }
                break;
                
            case GUISETLIST_LAYOUT_TEXT_IMAGE:
                if (setlist_ptr->is_need_arrow)
                {
                    arrow_rect = GetArrowRect(setlist_ptr, TRUE);
                    text_rect.left = arrow_rect.left + 1;
                }
                
                image_rect = GetImageDispRect(setlist_ptr, item_info_ptr);
                text_rect.right = image_rect.left - 1;
                break;
                
            case GUISETLIST_LAYOUT_TEXT:         // 只有文本的时候,文本显示以align确定
                if (setlist_ptr->is_need_arrow)
                {
                    // 只显示文本的情况
                    arrow_rect = GetArrowRect(setlist_ptr, TRUE);
                    text_rect.left = arrow_rect.right + 1;
                    
                    arrow_rect = GetArrowRect(setlist_ptr, FALSE);
                    text_rect.right = arrow_rect.left - 1;
                }
                break;
                
            default :
                //SCI_TRACE_LOW:"GetTextRect: not surport text!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_1676_112_2_18_3_21_20_297,(uint8*)"");
                break;
            }
        }
        // 只显示文本的情况
        else if (GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_TEXT)
            && !GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_IMAGE))
        { 
            arrow_rect = GetArrowRect(setlist_ptr, TRUE);
            text_rect.left = arrow_rect.right + 1;
            
            arrow_rect = GetArrowRect(setlist_ptr, FALSE);
            text_rect.right = arrow_rect.left - 1;
        }
        // 只显示图片的情况
        else if (!GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_TEXT)
            && GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_IMAGE))
        { 
            text_rect.left = text_rect.right = 0;
            text_rect.top = text_rect.bottom = 0;
        }
    }
    else
    {
        text_rect.left += setlist_ptr->default_style.left_margin;
        text_rect.right -= setlist_ptr->default_style.right_margin;
    }

    if (GUI_IsInvalidRectEx(text_rect))
    {
        text_rect.left = text_rect.right = text_rect.top = text_rect.bottom = 0;
    }
    
    return text_rect;
}

/*****************************************************************************/
// Description : 获取箭头显示的区域
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetArrowRect(
                              const CTRLSETLIST_OBJ_T *setlist_ptr,
                              BOOLEAN is_left
                              )
{
    uint16 image_width = 0;
    uint16 image_height = 0;
    GUI_RECT_T arrow_rect = {0};
    GUI_RECT_T ctrl_rect = {0};
    BOOLEAN is_calc = FALSE; // 当id有效,的确计算了左右坐标了,这个置TRUE,才能计算上下坐标

    if (PNULL == setlist_ptr)
    {
        //SCI_TRACE_LOW:"GetArrowRect error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_1719_112_2_18_3_21_20_298,(uint8*)"");
        return arrow_rect;
    }

    ctrl_rect = AdjustSetlistRect(setlist_ptr);

    // 计算左右坐标
    if (is_left && MMITHEME_CheckImageID(setlist_ptr->default_style.left_arrow_icon_up))
    {
        GUIRES_GetImgWidthHeight(&image_width, &image_height, 
            setlist_ptr->default_style.left_arrow_icon_up, setlist_ptr->win_id);

        arrow_rect.left = ctrl_rect.left;
        arrow_rect.right = arrow_rect.left + image_width - 1;

        is_calc = TRUE;
    }
    else if (!is_left && MMITHEME_CheckImageID(setlist_ptr->default_style.right_arrow_icon_up))
    {
        GUIRES_GetImgWidthHeight(&image_width, &image_height, 
            setlist_ptr->default_style.right_arrow_icon_up, setlist_ptr->win_id);

        arrow_rect.right = ctrl_rect.right;
        arrow_rect.left = arrow_rect.right - image_width + 1;

        is_calc = TRUE;
    }
    
    // 计算上下坐标
    if (is_calc)
    {
        if (image_height < ctrl_rect.bottom - ctrl_rect.top + 1)
        {
            arrow_rect.top = (ctrl_rect.bottom + ctrl_rect.top + 1 - image_height)/2;
            arrow_rect.bottom = arrow_rect.top + image_height - 1;
        }
        else
        {
            arrow_rect.top = ctrl_rect.top;
            arrow_rect.bottom = ctrl_rect.bottom;
        }
    }

    return arrow_rect;
}

/*****************************************************************************/
// Description : 画背景
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DrawSetlistArrow(
                            CTRLSETLIST_OBJ_T   *setlist_ptr,
                            BOOLEAN             is_left, // 是否左侧，TRUE左侧，FALSE右侧
                            BOOLEAN             is_up // 是否弹起，TRUE弹起，FALSE按下
                            )
{
    GUI_RECT_T          arrow_rect = {0};
    MMI_IMAGE_ID_T      image_id = 0;
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T *    ctrl_ptr = (CTRLBASE_OBJ_T *)setlist_ptr;
    if (TypeOfSetList((CTRLBASE_OBJ_T*)setlist_ptr) && setlist_ptr->is_need_arrow)
    {
        arrow_rect = GetArrowRect(setlist_ptr, is_left);
        if (is_left)  // 左侧箭头
        {
            if (is_up) // 是否弹起
            {
                image_id = setlist_ptr->default_style.left_arrow_icon_up;
            }
            else
            {
                image_id = setlist_ptr->default_style.left_arrow_icon_down;
            }
        }
        else // 右侧箭头
        {
            if (is_up) // 是否弹起
            {
                image_id = setlist_ptr->default_style.right_arrow_icon_up;
            }
            else
            {
                image_id = setlist_ptr->default_style.right_arrow_icon_down;
            }
        }

        if (MMITHEME_CheckImageID(image_id))
        {
            GUI_RECT_T      display_rect = {0};
            GUI_RECT_T      image_rect = {0};

            if(GUI_IntersectRect(&display_rect, ctrl_ptr->display_rect, arrow_rect))
            {
                //set image rect
                image_rect.left   = (int16)(display_rect.left - arrow_rect.left);
                image_rect.top    = (int16)(display_rect.top - arrow_rect.top);
                image_rect.right  = (int16)(image_rect.left + display_rect.right - display_rect.left);
                image_rect.bottom = (int16)(image_rect.top + display_rect.bottom - display_rect.top);
            
                // 画背景图片
                IMG_EnableTransparentColor(TRUE); 

                //set bg display info
                bg_display.is_transparent = FALSE;
                bg_display.rect           = ctrl_ptr->rect;
                bg_display.display_rect   = display_rect;
                bg_display.img_rect       = image_rect;
                bg_display.ctrl_handle    = ctrl_ptr->handle;
                bg_display.win_handle     = setlist_ptr->win_id;

                GUI_DisplayBg(&setlist_ptr->default_style.bg_info,
                    &bg_display,
                    &(ctrl_ptr->lcd_dev_info));
                
                GUIRES_DisplayImg(PNULL,
                    &display_rect,
                    &image_rect,
                    setlist_ptr->win_id,
                    image_id,
                    &ctrl_ptr->lcd_dev_info);

                IMG_EnableTransparentColor(FALSE);
            }
        }
    }
}

/*****************************************************************************/
// Description : 调整控件的区域
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DrawSetlistFrame(
                            CTRLSETLIST_OBJ_T *setlist_ptr
                            )
{
	CTRLBASE_OBJ_T * ctrl_ptr = (CTRLBASE_OBJ_T *)setlist_ptr;
    if(PNULL != setlist_ptr)
    {
        GUI_DisplayBorder(ctrl_ptr->rect, 
            ctrl_ptr->display_rect,
            &setlist_ptr->default_style.border,
            &ctrl_ptr->lcd_dev_info);
    }
    else
    {
        //SCI_TRACE_LOW:"DrawSetlistFrame error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_1863_112_2_18_3_21_21_299,(uint8*)"");
    }

    return;
}

/*****************************************************************************/
// Description : 调整控件的区域
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL GUI_RECT_T AdjustSetlistRect(
                                   const CTRLSETLIST_OBJ_T *setlist_ptr
                                   )
{
    BOOLEAN     is_border = TRUE;
    GUI_RECT_T  disp_rect = {0};
	CTRLBASE_OBJ_T * ctrl_ptr = (CTRLBASE_OBJ_T *)setlist_ptr;

    if (PNULL == setlist_ptr)
    {
        //SCI_TRACE_LOW:"AdjustSetlistRect error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_1882_112_2_18_3_21_21_300,(uint8*)"");
        return disp_rect;
    }

    //text is exist border
    if (GUI_BORDER_NONE == setlist_ptr->default_style.border.type)
    {
        is_border = FALSE;
    }

    if (is_border)
    {
        disp_rect = ctrl_ptr->rect;

        disp_rect.left += setlist_ptr->default_style.border.width;
        disp_rect.top += setlist_ptr->default_style.border.width;
        disp_rect.right -= setlist_ptr->default_style.border.width;
        disp_rect.bottom -= setlist_ptr->default_style.border.width;
    }
    else
    {
        disp_rect = ctrl_ptr->rect;
    }

    return disp_rect;
}

/*****************************************************************************/
// Description : destroy item data
// Global resource dependence : 
// Author:xiaoqing.lu
// Note: //is destroy anim ctrl
/*****************************************************************************/
LOCAL BOOLEAN DestroySetlistItemData(
                                     CTRLSETLIST_OBJ_T *setlist_ptr,
                                     GUISETLIST_ITEM_INFO_T *item_info_ptr
                                     )
{
    BOOLEAN     result = FALSE;
    
    if(PNULL != setlist_ptr && PNULL != item_info_ptr && PNULL != item_info_ptr->add_data )
    {
        switch(item_info_ptr->image_info.image_data_type)
        {
        case GUISETLIST_DATA_ANIM_DATA:
        case GUISETLIST_DATA_ANIM_PATH:
            MMK_DestroyControl(GUICTRL_GetCtrlHandle((IGUICTRL_T*)item_info_ptr->add_data));
            
            item_info_ptr->add_data = 0;
            result = TRUE;
            break;
            
        default:
            //SCI_TRACE_LOW:"DestroySetlistItemData: item_data_type %d is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_1933_112_2_18_3_21_21_301,(uint8*)"d", item_info_ptr->image_info.image_data_type);
            break;
        }
    }
    
    return (result);
}

/*****************************************************************************/
// Description : 画Items
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:注意:只画当前一个item
/*****************************************************************************/
LOCAL void DrawItemImage(
                         CTRLSETLIST_OBJ_T *setlist_ptr,
                         GUISETLIST_ITEM_INFO_T *item_info_ptr
                         )
{
    GUI_RECT_T  disp_rect = {0};
    GUI_RECT_T  cross_rect = {0};
	CTRLBASE_OBJ_T * ctrl_ptr = (CTRLBASE_OBJ_T *)setlist_ptr;

    // 获取图片的区域
    disp_rect = GetImageDispRect(setlist_ptr, item_info_ptr);

    // 检查区域
    if (PNULL == setlist_ptr || GUI_IsRectEmpty(disp_rect))
    {
        return;
    }

    if(GUI_IntersectRect(&cross_rect, ctrl_ptr->display_rect, disp_rect))
    {
        // 开始绘图
        switch( item_info_ptr->image_info.image_data_type )
        {
        case GUISETLIST_DATA_IMAGE_ID:
            if (MMITHEME_CheckImageID(item_info_ptr->image_info.image_data.image_id))
            {
                GUI_RECT_T      image_rect = {0};
            
                // 计算背景图片现实的区域
                image_rect.bottom = image_rect.top + (cross_rect.bottom - cross_rect.top + 1);
                image_rect.right  = image_rect.left + (cross_rect.right - cross_rect.left + 1);
            
                // 画背景图片
                IMG_EnableTransparentColor(TRUE); 
                GUIRES_DisplayImg(PNULL,
                    &cross_rect,
                    &image_rect,
                    setlist_ptr->win_id,
                    item_info_ptr->image_info.image_data.image_id,
                    &ctrl_ptr->lcd_dev_info);
                IMG_EnableTransparentColor(FALSE); 
            }
            break;
        
        case GUISETLIST_DATA_ANIM_DATA:
        case GUISETLIST_DATA_ANIM_PATH:
            {
                MMI_CONTROL_CREATE_T    create = {0};
                GUIANIM_INIT_DATA_T     init_data = {0};
                GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_FAIL;
                GUIANIM_CTRL_INFO_T     ctrl_info = {0};
                GUIANIM_DISPLAY_INFO_T  display_info = {0};
            
                if(PNULL == item_info_ptr->add_data)
                {
                    init_data.both_rect.h_rect = init_data.both_rect.v_rect = disp_rect;
                    create.ctrl_id = 0;
                    create.guid    = SPRD_GUI_ANIM_ID;
                    create.parent_win_handle = setlist_ptr->win_id;
                    create.parent_ctrl_handle = ctrl_ptr->handle;
                    create.init_data_ptr = &init_data;
                    item_info_ptr->add_data = (uint32)MMK_CreateControl( &create );
                    
                    SCI_ASSERT( PNULL != item_info_ptr->add_data ); /*assert verified*/
                    
                    // 设置动画控件显示的默认图片
                    GUIANIM_SetDefaultIconByPtr(
                        (void*)item_info_ptr->add_data, 
                        MMITHEME_GetDefaultItemIcon(), 
                        MMITHEME_GetDefaultItemIcon()
                        );
                    
                    //set anim param
                    ctrl_info.ctrl_ptr   = (void*)item_info_ptr->add_data;
                    
                    display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
                    display_info.is_play_once      = FALSE;
                    display_info.is_syn_decode     = FALSE;
                    display_info.is_update         = TRUE;
                    display_info.is_disp_one_frame = TRUE;
                    display_info.bg.bg_type        = GUI_BG_COLOR;
                    display_info.bg.color          = setlist_ptr->default_style.bg_info.color;
                    
                    // 设置同步解码
                    display_info.is_syn_decode = TRUE;
                    
                    // 显示动画
                    if(GUISETLIST_DATA_ANIM_DATA == item_info_ptr->image_info.image_data_type)
                    {
                        anim_result = GUIANIM_SetParam(&ctrl_info, 
                            (GUIANIM_DATA_INFO_T*)&item_info_ptr->image_info.image_data.anim_data, PNULL, &display_info );
                    }
                    else
                    {
                        anim_result = GUIANIM_SetParam(&ctrl_info, PNULL,
                            (GUIANIM_FILE_INFO_T*)&item_info_ptr->image_info.image_data.anim_path, &display_info );
                    }
                }
                else
                {
                    //set anim param
                    ctrl_info.ctrl_ptr   = (void*)item_info_ptr->add_data;
                }

                GUIANIM_SetDisplayRectByPtr(ctrl_info.ctrl_ptr, &cross_rect,FALSE);
            }
            break;
        
        default:
            //SCI_TRACE_LOW:"DrawItemImage: type %d is not image!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_2053_112_2_18_3_21_21_302,(uint8*)"d", item_info_ptr->image_info.image_data_type);
            break;
        }
    }
}

/*****************************************************************************/
// Description : 画Item 文本
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void DrawItemText(
                        CTRLSETLIST_OBJ_T *setlist_ptr,
                        GUISETLIST_ITEM_INFO_T *item_info_ptr
                        )
{
    
    GUI_RECT_T      text_rect = {0};
    GUI_RECT_T      cross_rect = {0};
    GUISTR_STYLE_T  style_info = {0};
	CTRLBASE_OBJ_T * ctrl_ptr = (CTRLBASE_OBJ_T *)setlist_ptr;
    
    // 参数检查
    if (PNULL != setlist_ptr && PNULL != item_info_ptr)
    {
        style_info.align = setlist_ptr->default_style.align;
        style_info.font_color = setlist_ptr->default_style.font_color;
        style_info.font = setlist_ptr->default_style.font_type;
        style_info.char_space = setlist_ptr->default_style.char_space;
        style_info.effect = setlist_ptr->default_style.effect;
        
        // 显示文本
        text_rect = GetTextRect(setlist_ptr, item_info_ptr);

        // 检查区域
        if (GUI_IsRectEmpty(text_rect))
        {
            return;
        }

        if(GUI_IntersectRect(&cross_rect, ctrl_ptr->display_rect, text_rect))
        {
            if (GUISETLIST_TYPE_CHECKLIST == setlist_ptr->list_type)
            {
                if (setlist_ptr->is_disptxt_auto)
                {
                    if (0 == setlist_ptr->selected_cnt)
                    {
                        return;
                    }
                    
                    SetMultiDispText(setlist_ptr);
                }
            }
            else
            {
                if (setlist_ptr->is_disptxt_auto)
                {
                    SetDispTextInfo(setlist_ptr, &item_info_ptr->str_info);
                }
            }

            GUISTR_DrawTextToLCDInRect(&ctrl_ptr->lcd_dev_info, &text_rect,
                &cross_rect, &setlist_ptr->disp_str, &style_info, 
                setlist_ptr->default_style.str_state,
                GUISTR_TEXT_DIR_AUTO);
        }
    }

    return;
}

/*****************************************************************************/
// Description : 获取多选模式下要显示的text
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL void SetMultiDispText(CTRLSETLIST_OBJ_T *setlist_ptr)
{
    uint32 item_idx = 0;
    GUISETLIST_ITEM_INFO_T *item_info_ptr = PNULL;
    MMI_LINK_NODE_T *link_ptr = PNULL;
    uint32 buf_size = 0;
    uint32 select_cnt = 0;
    uint32 cur_len = 0;

    if (PNULL == setlist_ptr)
    {
        return;
    }

    ReleaseMultiTextInfo(setlist_ptr);

    if (0 == setlist_ptr->selected_cnt)
    {
        return;
    }
    
    setlist_ptr->disp_str.wstr_len = GetMultiDisTextLen(setlist_ptr);
    setlist_ptr->disp_str.wstr_len += (setlist_ptr->selected_cnt - 1);

    buf_size = setlist_ptr->disp_str.wstr_len;
    buf_size *= sizeof(wchar);

    setlist_ptr->disp_str.wstr_ptr = SCI_ALLOC_APP(buf_size);
    
    SCI_MEMSET(setlist_ptr->disp_str.wstr_ptr, 0, buf_size);

    for (item_idx = 0; item_idx < setlist_ptr->item_info.item_total_num; item_idx++)
    {
        link_ptr = SearchCurVisibleItem(setlist_ptr, item_idx);

        if (PNULL == link_ptr) continue; 

        item_info_ptr = (GUISETLIST_ITEM_INFO_T*)link_ptr->data;

        if (GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_INVISIBLE))
        {
            continue;
        }
            
        if (!item_info_ptr->is_selected) continue;

        select_cnt++;

        if (PNULL == item_info_ptr->str_info.wstr_ptr) continue;
        
        SCI_MEMCPY(setlist_ptr->disp_str.wstr_ptr + cur_len,
            item_info_ptr->str_info.wstr_ptr,
            item_info_ptr->str_info.wstr_len * sizeof(wchar));

        cur_len += item_info_ptr->str_info.wstr_len;

        if (select_cnt < setlist_ptr->selected_cnt)
        {
            setlist_ptr->disp_str.wstr_ptr[cur_len++] = ',';
        }
    }

    return;
}

/*****************************************************************************/
// Description : 获取多选模式下要显示的text的长度
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL uint16 GetMultiDisTextLen(CTRLSETLIST_OBJ_T *setlist_ptr)
{
    uint32 item_idx = 0;
    GUISETLIST_ITEM_INFO_T *item_info_ptr = PNULL;
    uint16 str_len = 0;
    MMI_LINK_NODE_T *link_ptr = PNULL;

    if (PNULL == setlist_ptr)
    {
        return str_len;
    }

    for (item_idx = 0; item_idx < setlist_ptr->item_info.item_total_num; item_idx++)
    {
        link_ptr = SearchCurVisibleItem(setlist_ptr, item_idx);

        if (PNULL == link_ptr) continue;
        
        item_info_ptr = (GUISETLIST_ITEM_INFO_T*)link_ptr->data;

        if (GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_INVISIBLE))
        {
            continue;
        }

        if (!item_info_ptr->is_selected) continue;
        
        if (PNULL == item_info_ptr->str_info.wstr_ptr) continue;

        str_len += item_info_ptr->str_info.wstr_len;
    }

    return str_len;
    
}

/*****************************************************************************/
//  Description : 获取选中item的index
//  Global resource dependence : 
//  Author:jin.wang
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSelectedItem(
    const CTRLSETLIST_OBJ_T *list_ptr,
    uint16 array_len,
    uint16 *idx_ptr //[in/out]:index array,may PNULL
)
{
    uint32 item_idx = 0;
    uint16 selected_cnt = 0;
    MMI_LINK_NODE_T *node_ptr = PNULL;
    GUISETLIST_ITEM_INFO_T *item_info_ptr = PNULL;

    if (PNULL == list_ptr || (!TypeOfSetList((CTRLBASE_OBJ_T*)list_ptr)))
    {
        return selected_cnt;
    }
    
    for (item_idx = 0; item_idx < list_ptr->item_info.item_total_num; item_idx++ )
    {       
        node_ptr = SearchItemByIndex(list_ptr, item_idx);

        if (PNULL == node_ptr)
        {
            continue;
        }
        
        item_info_ptr = (GUISETLIST_ITEM_INFO_T*)node_ptr->data;
        
        if (PNULL == item_info_ptr)
        {
            continue;
        }

        if (item_info_ptr->is_selected)
        {
            if (selected_cnt < array_len)
            {
                if (PNULL != idx_ptr)
                {
                    idx_ptr[selected_cnt] = item_idx;
                }
            }
            
            selected_cnt++;
        }
    }
    
    return selected_cnt;
}

/*****************************************************************************/
// Description : release title
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: 
/*****************************************************************************/
LOCAL void ReleaseSetlistTitleInfo(
                                   CTRLSETLIST_OBJ_T *setlist_ptr
                                   )
{
    if (PNULL != setlist_ptr && PNULL != setlist_ptr->title_str_info.wstr_ptr)
    {
        SCI_FREE(setlist_ptr->title_str_info.wstr_ptr);
        setlist_ptr->title_str_info.wstr_ptr = PNULL;
        setlist_ptr->title_str_info.wstr_len = 0;
    }

    return;
}

/*****************************************************************************/
// Description : release multi text
// Global resource dependence : 
// Author: jin.wang
// Note: 
/*****************************************************************************/
LOCAL void ReleaseMultiTextInfo(CTRLSETLIST_OBJ_T *setlist_ptr)
{
    if (PNULL != setlist_ptr && PNULL != setlist_ptr->disp_str.wstr_ptr)
    {
        SCI_FREE(setlist_ptr->disp_str.wstr_ptr);
        setlist_ptr->disp_str.wstr_len = 0;
    }

    return;
}

/*****************************************************************************/
// Description : set title
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: if is_set = TRUE, set title; else if is_set = FALSE, release title
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetTitleInfo(
                                        MMI_CTRL_ID_T ctrl_id,
                                        MMI_STRING_T *str_ptr,
                                        BOOLEAN is_set
                                        )
{

    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);

    return SetThisCtrlTitleInfo(setlist_ptr, str_ptr, is_set);
}

/*****************************************************************************/
// Description : set title
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetTitleTextId(
                                         MMI_CTRL_ID_T ctrl_id,
                                         MMI_TEXT_ID_T text_id
                                         )
{
    MMI_STRING_T    text = {0};
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);

    MMITHEME_GetResText(text_id, ctrl_id, &text);
    
    return SetThisCtrlTitleInfo(setlist_ptr, &text, TRUE);
}

/*****************************************************************************/
// Description : set title
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: if is_set = TRUE, set title; else if is_set = FALSE, release title
/*****************************************************************************/
LOCAL BOOLEAN SetThisCtrlTitleInfo(
                                   CTRLSETLIST_OBJ_T *setlist_ptr,
                                   MMI_STRING_T *str_ptr,
                                   BOOLEAN is_set
                                   )
{ 
    BOOLEAN result = FALSE;

    if (PNULL != setlist_ptr)
    {
        if (!is_set)
        {
            if (PNULL != setlist_ptr->title_str_info.wstr_ptr)
            {
                ReleaseSetlistTitleInfo(setlist_ptr);

                result = TRUE;
            }
        }
        else
        {
            if (PNULL != str_ptr && PNULL != str_ptr->wstr_ptr && 0 != str_ptr->wstr_len)
            {
                ReleaseSetlistTitleInfo(setlist_ptr);

                setlist_ptr->title_str_info.wstr_ptr = SCI_ALLOC_APP(sizeof(wchar) * str_ptr->wstr_len);

                SCI_MEMSET(setlist_ptr->title_str_info.wstr_ptr, 0, sizeof(wchar) * str_ptr->wstr_len);

                SCI_MEMCPY(setlist_ptr->title_str_info.wstr_ptr, str_ptr->wstr_ptr, sizeof(wchar) * str_ptr->wstr_len);
                
                setlist_ptr->title_str_info.wstr_len = str_ptr->wstr_len;

                result = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
// Description : 画Items
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:注意:只画当前一个item
/*****************************************************************************/
LOCAL void DrawSetlistItem(
                           CTRLSETLIST_OBJ_T    *setlist_ptr
                           )
{
    GUI_RECT_T              cross_rect = {0};
    MMI_LINK_NODE_T         *node_ptr = PNULL;
    GUISETLIST_ITEM_INFO_T  *item_info_ptr = PNULL;
	CTRLBASE_OBJ_T *        ctrl_ptr = (CTRLBASE_OBJ_T *)setlist_ptr;

    if (TypeOfSetList((CTRLBASE_OBJ_T*)setlist_ptr)
        && GUI_IntersectRect(&cross_rect, ctrl_ptr->display_rect, ctrl_ptr->rect))
    {
        // 画背景
        DrawSetlistBg(setlist_ptr);

        // 画边框
        DrawSetlistFrame(setlist_ptr);

        // 画箭头
        DrawSetlistArrow(setlist_ptr, TRUE, TRUE); // 左侧up箭头
        DrawSetlistArrow(setlist_ptr, FALSE, TRUE); // 右侧up箭头

        // 查找一个节点, 从当前点开始,循环查找;这个节点必然可见，如果列表中都是不可见的，则返回NULL
        // 同时,这里会设置当前有效的结点索引号,
        node_ptr = SearchCurVisibleItem(setlist_ptr, setlist_ptr->cur_item_index);
        // node_ptr = SearchItemByIndex(setlist_ptr, setlist_ptr->cur_item_index);

        if (PNULL != node_ptr
            && !GetState(((GUISETLIST_ITEM_INFO_T*)node_ptr->data)->item_state, GUISETLIST_ITEM_STATE_INVISIBLE))
        {
            item_info_ptr = (GUISETLIST_ITEM_INFO_T*)node_ptr->data;

            // 处理显示内容
            if (PNULL != item_info_ptr)
            {
                // 即显示图片，又显示文本的情况
                if (GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_TEXT)  
                    && GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_IMAGE))
                { 
                    // 显示图片
                    DrawItemImage(setlist_ptr, item_info_ptr);

                    // 显示文本
                    DrawItemText(setlist_ptr, item_info_ptr);
                }

                // 只显示文本的情况
                else if (GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_TEXT) 
                    && !GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_IMAGE))
                { 
                    DrawItemText(setlist_ptr, item_info_ptr);
                }

                 // 只显示图片的情况
                else if (!GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_TEXT)
                    && GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_IMAGE))
                { 
                    // 显示图片
                    DrawItemImage(setlist_ptr, item_info_ptr);
                }
            }
        }
    }
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// Description : 处理触笔按下的消息
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetListTpDown(
                                       CTRLSETLIST_OBJ_T *setlist_ptr, 
                                       GUI_POINT_T *point_ptr
                                       )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    GUI_RECT_T left_arrow_rect = {0};
    GUI_RECT_T right_arrow_rect = {0};
    GUI_RECT_T item_valid_rect = {0};

    if (setlist_ptr->is_need_arrow)
    {
        item_valid_rect = AdjustSetlistRect(setlist_ptr);
        
        // 箭头区域
        left_arrow_rect = GetArrowRect(setlist_ptr, TRUE);
        right_arrow_rect = GetArrowRect(setlist_ptr, FALSE);
        
        if (setlist_ptr->is_need_arrow)
        {
            item_valid_rect.left += left_arrow_rect.right - left_arrow_rect.left;
            item_valid_rect.right -= right_arrow_rect.right - right_arrow_rect.left;
        }
        
        // 恢复
        setlist_ptr->right_is_pressed = FALSE;
        setlist_ptr->left_is_pressed = FALSE;
        
        // 判断在哪个区域
        if (GUI_PointIsInRect(*point_ptr, left_arrow_rect)) // 左侧down箭头
        {
            setlist_ptr->left_is_pressed = TRUE;
            DrawSetlistArrow(setlist_ptr, TRUE, FALSE); 
        }
        else if (GUI_PointIsInRect(*point_ptr, right_arrow_rect)) // 左侧down箭头
        {
            setlist_ptr->right_is_pressed = TRUE;
            DrawSetlistArrow(setlist_ptr, FALSE, FALSE); 
        }
        else
        {
            SCI_TRACE_LOW("HandleSetListTpDown in item rect!");  // 暂时不做处理
        }
    }

    return result;
}
#endif

/*****************************************************************************/
// Description : open win
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL void OpenSetlistDispWin(
                              CTRLSETLIST_OBJ_T *setlist_ptr
                              )
{
    MMI_OPEN_SETLIST_WIN_T setlist_data = {0};
    CTRLBASE_OBJ_T *        ctrl_ptr = (CTRLBASE_OBJ_T *)setlist_ptr;

    setlist_data.parant_win_handle = setlist_ptr->win_id;
    setlist_data.parant_ctrl_handle = ctrl_ptr->handle;  
    setlist_data.cur_item_index = setlist_ptr->cur_item_index;
    setlist_data.font_color = setlist_ptr->default_style.font_color;
    setlist_data.list_font = setlist_ptr->default_style.font_type;
    setlist_data.check_allow_max_num = setlist_ptr->item_info.item_total_num;

    if (GUISETLIST_TYPE_RADIOLIST == setlist_ptr->list_type)
    {
        setlist_data.list_type = GUILIST_RADIOLIST_E;
    }
    else
    {
        setlist_data.list_type = GUILIST_CHECKLIST_E;
    }

    MMITHEME_CreateSetlistChildWin(&setlist_data);
 
    return;
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// Description : 处理触笔弹起的消息
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetListTpUp(
                                     CTRLSETLIST_OBJ_T  *setlist_ptr, 
                                     DPARAM             param
                                     )
{
    MMI_RESULT_E    result              = MMI_RESULT_TRUE;
    GUI_RECT_T      left_arrow_rect     = {0};
    GUI_RECT_T      right_arrow_rect    = {0};
    GUI_RECT_T      item_valid_rect     = {0};
    BOOLEAN         is_cyc              = FALSE; // 是否循环搜索
    BOOLEAN         is_next             = FALSE;
    BOOLEAN         is_item_press       = FALSE;
    uint32          pre_item_index      = 0;
    GUI_POINT_T     tp_point            = {0};    
    CTRLBASE_OBJ_T  *ctrl_ptr           = PNULL;
    //set tp press point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    item_valid_rect = AdjustSetlistRect(setlist_ptr);

	if (PNULL == setlist_ptr)
	{
        return MMI_RESULT_FALSE;
	}

    ctrl_ptr = (CTRLBASE_OBJ_T  *)setlist_ptr;
    if (setlist_ptr->is_need_arrow) // 支持左右箭头时的处理
    {
        // 箭头区域
        left_arrow_rect = GetArrowRect(setlist_ptr, TRUE);
        right_arrow_rect = GetArrowRect(setlist_ptr, FALSE);
        
        if (setlist_ptr->is_need_arrow)
        {
            item_valid_rect.left += left_arrow_rect.right - left_arrow_rect.left;
            item_valid_rect.right -= right_arrow_rect.right - right_arrow_rect.left;
        }
        
        is_cyc = GetState(setlist_ptr->ctrl_state, GUISETLIST_STATE_ISCYC);
        
        pre_item_index = setlist_ptr->cur_item_index;
        
        if (GUI_PointIsInRect(tp_point, left_arrow_rect))
        {
            if ((!MMK_GET_TP_SLIDE(param)) &&
                (PNULL != SearchPreVisibleItem(setlist_ptr, setlist_ptr->cur_item_index, is_cyc)))
            {
                MMI_LINK_NODE_T *tmp_node_ptr = SearchItemByIndex(setlist_ptr, pre_item_index);
                
                if (PNULL != tmp_node_ptr)
                {
                    DestroySetlistItemData(setlist_ptr, (GUISETLIST_ITEM_INFO_T*)tmp_node_ptr->data);
                }
                
                GUICTRL_PostNotifyEx(ctrl_ptr->handle,MSG_CTL_SETLIST_SWITCH,&is_next,sizeof(BOOLEAN));
                
                DrawSetlistItem(setlist_ptr);   // 如果下一个找到, 显示下一个
            }
            else
            {
                DrawSetlistArrow(setlist_ptr, TRUE, TRUE);  // 如果找不到,显示弹起的图片
            }
        }
        else if (GUI_PointIsInRect(tp_point, right_arrow_rect))
        {
            if ((!MMK_GET_TP_SLIDE(param)) && 
                (PNULL != SearchNextVisibleItem(setlist_ptr, setlist_ptr->cur_item_index, is_cyc)))
            {
                MMI_LINK_NODE_T *tmp_node_ptr = SearchItemByIndex(setlist_ptr, pre_item_index);
                
                if (PNULL != tmp_node_ptr)
                {
                    DestroySetlistItemData(setlist_ptr, (GUISETLIST_ITEM_INFO_T*)tmp_node_ptr->data);
                }
                
                is_next = TRUE;
                GUICTRL_PostNotifyEx(ctrl_ptr->handle,MSG_CTL_SETLIST_SWITCH,&is_next,sizeof(BOOLEAN));
                
                DrawSetlistItem(setlist_ptr); // 如果找到下一个,显示下一个
            }
            else
            {
                DrawSetlistArrow(setlist_ptr, FALSE, TRUE); // 如果找不到小,显示弹起的图片
            }
        }
        else if (GUI_PointIsInRect(tp_point, item_valid_rect))
        {
            if (setlist_ptr->left_is_pressed)
            {
                DrawSetlistArrow(setlist_ptr, TRUE, TRUE);
            }
            else if (setlist_ptr->right_is_pressed)
            {
                DrawSetlistArrow(setlist_ptr, FALSE, TRUE);
            }
            
            is_item_press = TRUE;
        }
        else
        {
            if (setlist_ptr->left_is_pressed)
            {
                DrawSetlistArrow(setlist_ptr, TRUE, TRUE);
            }
            else if (setlist_ptr->right_is_pressed)
            {
                DrawSetlistArrow(setlist_ptr, FALSE, TRUE);
            }
            else
            {
                //SCI_TRACE_LOW:"HandleSetListTpUp NULL!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_2655_112_2_18_3_21_22_303,(uint8*)"");
            }
        }
    }
    else
    {
        // 判断是否在有效区域内
        if (GUI_PointIsInRect(tp_point, item_valid_rect))
        {
            if (setlist_ptr->left_is_pressed)
            {
                DrawSetlistArrow(setlist_ptr, TRUE, TRUE);
            }
            else if (setlist_ptr->right_is_pressed)
            {
                DrawSetlistArrow(setlist_ptr, FALSE, TRUE);
            }
        
            is_item_press = TRUE;
        }
        else if ((!GUI_IsRectEmpty(setlist_ptr->tp_rect)) &&
             (GUI_PointIsInRect(tp_point,setlist_ptr->tp_rect)))
        {
            is_item_press = TRUE;
        }
    }
    
    setlist_ptr->left_is_pressed = FALSE;
    setlist_ptr->right_is_pressed = FALSE;
    
    //not slide
    if ((is_item_press) &&
        (!MMK_GET_TP_SLIDE(param)))
    {
        HandleSetListWebKeyDown(setlist_ptr);
    } 

    return result;
}
#endif

/*****************************************************************************/
// Description : handle MSG_KEYDOWN_LEFT message.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetListLeftKeyDown(
                                            CTRLSETLIST_OBJ_T *setlist_ptr
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    BOOLEAN         is_cyc = FALSE;
    BOOLEAN         is_next = FALSE;
    uint32          pre_item_index = 0;
	CTRLBASE_OBJ_T *        ctrl_ptr = (CTRLBASE_OBJ_T *)setlist_ptr;

    if (PNULL == setlist_ptr)
    {
        //SCI_TRACE_LOW:"HandleSetListLeftKeyDown error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_2710_112_2_18_3_21_23_304,(uint8*)"");
        return result;
    }

    is_cyc = GetState(setlist_ptr->ctrl_state, GUISETLIST_STATE_ISCYC);

    pre_item_index = setlist_ptr->cur_item_index;

    if (PNULL != SearchPreVisibleItem(setlist_ptr, setlist_ptr->cur_item_index, is_cyc))
    {
        MMI_LINK_NODE_T *tmp_node_ptr = SearchItemByIndex(setlist_ptr, pre_item_index);
        
        if (PNULL != tmp_node_ptr)
        {
            DestroySetlistItemData(setlist_ptr, (GUISETLIST_ITEM_INFO_T*)tmp_node_ptr->data);
        }

        GUICTRL_PostNotifyEx(ctrl_ptr->handle,MSG_CTL_SETLIST_SWITCH,&is_next,sizeof(BOOLEAN));

        DrawSetlistItem(setlist_ptr);

        result = MMI_RESULT_TRUE;
    }
    
    return(result);
}

/*****************************************************************************/
// Description : handle MSG_KEYDOWN_RIGHT message.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetListRightKeyDown(
                                             CTRLSETLIST_OBJ_T *setlist_ptr
                                             )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    BOOLEAN         is_cyc = FALSE;
    BOOLEAN         is_next = TRUE;
    uint32          pre_item_index = 0;
	CTRLBASE_OBJ_T *        ctrl_ptr = (CTRLBASE_OBJ_T *)setlist_ptr;

    if (PNULL == setlist_ptr)
    {
        //SCI_TRACE_LOW:"HandleSetListRightKeyDown error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_2752_112_2_18_3_21_23_305,(uint8*)"");
        return result;
    }

    is_cyc = GetState(setlist_ptr->ctrl_state, GUISETLIST_STATE_ISCYC);

    pre_item_index = setlist_ptr->cur_item_index;
    
    if (PNULL != SearchNextVisibleItem(setlist_ptr, setlist_ptr->cur_item_index, is_cyc))
    {
        MMI_LINK_NODE_T *tmp_node_ptr = SearchItemByIndex(setlist_ptr, pre_item_index);
        
        if (PNULL != tmp_node_ptr)
        {
            DestroySetlistItemData(setlist_ptr, (GUISETLIST_ITEM_INFO_T*)tmp_node_ptr->data);
        }

        GUICTRL_PostNotifyEx(ctrl_ptr->handle,MSG_CTL_SETLIST_SWITCH,&is_next,sizeof(BOOLEAN));
        
        DrawSetlistItem(setlist_ptr);

        result = MMI_RESULT_TRUE;
    }
    
    return(result);
}

/*****************************************************************************/
// Description : handle MSG_KEYDOWN_WEB message.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetListWebKeyDown(
                                           CTRLSETLIST_OBJ_T *setlist_ptr
                                           )
{
    BOOLEAN         is_next = TRUE;
    uint32          item_num = 0;
    uint32          pre_item_index = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
	CTRLBASE_OBJ_T *        ctrl_ptr = (CTRLBASE_OBJ_T *)setlist_ptr;

    if (PNULL == setlist_ptr)
    {
        //SCI_TRACE_LOW:"HandleSetListWebKeyDown error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_2794_112_2_18_3_21_23_306,(uint8*)"");
        return result;
    }

    item_num = CountVisibleItemNum(setlist_ptr);

    // 记录一下
    pre_item_index = setlist_ptr->cur_item_index;

    if ((VISIBLE_ITEM_NUM_1 == item_num || VISIBLE_ITEM_NUM_2 == item_num)
        && !GetState(setlist_ptr->ctrl_state, GUISETLIST_STATE_OPEN_POPWIN)) // 只有两个
    {
        if (VISIBLE_ITEM_NUM_1 != item_num 
            && SearchNextVisibleItem(setlist_ptr, setlist_ptr->cur_item_index, TRUE))
        {
            MMI_LINK_NODE_T *tmp_node_ptr = SearchItemByIndex(setlist_ptr, pre_item_index);
        
            if (PNULL != tmp_node_ptr)
            {
                DestroySetlistItemData(setlist_ptr, (GUISETLIST_ITEM_INFO_T*)tmp_node_ptr->data);
            }

            GUICTRL_PostNotifyEx(ctrl_ptr->handle,MSG_CTL_SETLIST_SWITCH,&is_next,sizeof(BOOLEAN));
            
            DrawSetlistItem(setlist_ptr);
        }
    }
    else
    {
        if (!GetState(setlist_ptr->ctrl_state, GUISETLIST_STATE_USER_OPEN)
            && GetState(setlist_ptr->ctrl_state, GUISETLIST_STATE_OPEN_POPWIN))
        {
            MMK_PostMsg(ctrl_ptr->handle,MSG_CTL_SETLIST_OPENWIN, PNULL, 0);
            
            // OpenSetlistDispWin(setlist_ptr); 
        }
        else
        {
            GUICTRL_PostNotifyEx(ctrl_ptr->handle, MSG_NOTIFY_MIDSK, PNULL, 0 );
        }
    }
    
    return(result);
}

/*****************************************************************************/
// Description : set control display rect
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDisplayRect(
                             CTRLBASE_OBJ_T       *base_ctrl_ptr,  //in:
                             const GUI_RECT_T *rect_ptr,      //in:
                             BOOLEAN          is_update       //in:
                             )
{
    BOOLEAN     result = FALSE;
    CTRLSETLIST_OBJ_T   *setlist_ptr = (CTRLSETLIST_OBJ_T*)base_ctrl_ptr;

    if (PNULL != base_ctrl_ptr && TypeOfSetList(base_ctrl_ptr) && PNULL != rect_ptr)
    {
        base_ctrl_ptr->display_rect.left = rect_ptr->left;
        base_ctrl_ptr->display_rect.top = rect_ptr->top;
        base_ctrl_ptr->display_rect.right = rect_ptr->right;
        base_ctrl_ptr->display_rect.bottom = rect_ptr->bottom;

        result = TRUE;

        if (is_update)
        {
            DrawSetlistItem(setlist_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
// Description : get control display rect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayRect(
                             CTRLBASE_OBJ_T   *base_ctrl_ptr,  //in:
                             GUI_RECT_T   *rect_ptr       //in/out:
                             )
{
    BOOLEAN     result = FALSE;
    CTRLSETLIST_OBJ_T   *setlist_ptr = (CTRLSETLIST_OBJ_T*)base_ctrl_ptr;

    if (PNULL != base_ctrl_ptr && TypeOfSetList(base_ctrl_ptr) && PNULL != rect_ptr)
    {
        if (GUI_IsRectEmpty(setlist_ptr->tp_rect))
        {
            *rect_ptr = base_ctrl_ptr->display_rect;
        }
        else
        {
            *rect_ptr = setlist_ptr->tp_rect;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : set control boder info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SetBorder(
                        CTRLBASE_OBJ_T    *base_ctrl_ptr,
                        GUI_BORDER_T  *border_ptr
                        )
{
    BOOLEAN result = FALSE;
    CTRLSETLIST_OBJ_T *setlist_ptr = (CTRLSETLIST_OBJ_T*)base_ctrl_ptr;

    if (PNULL != border_ptr && PNULL != setlist_ptr)
    {
        result = TRUE;
        setlist_ptr->default_style.border.color = border_ptr->color;
        setlist_ptr->default_style.border.type = border_ptr->type;
        setlist_ptr->default_style.border.width = border_ptr->width;
    }

    return (result);
}

/*****************************************************************************/
// Description : set set list is circular handle left/right key msg
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SetCircularHandleLeftRight(
                                         CTRLBASE_OBJ_T *ctrl_ptr,
                                         BOOLEAN    is_circular
                                         )
{
    BOOLEAN             result = TRUE;
    CTRLSETLIST_OBJ_T   *setlist_ptr = PNULL;

    setlist_ptr = (CTRLSETLIST_OBJ_T *)ctrl_ptr;

    SetState(&setlist_ptr->ctrl_state,GUISETLIST_STATE_ISCYC,is_circular);

    return (result);
}

/*****************************************************************************/
// Description : get control height by width,include content,border etc.
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN GetSetListHeight(
                               CTRLBASE_OBJ_T     *base_ctrl_ptr,  //in:
                               uint16         width,          //in:
                               uint16         *height_ptr     //in/out:
                               )
{
    BOOLEAN             result = TRUE;
    BOOLEAN             is_border = TRUE;
    uint16              font_height = 0;
    uint16              line_height = 0;
    uint16              image_width = 0;
    uint16              image_height = 0;
    CTRLSETLIST_OBJ_T   *setlist_ptr = PNULL;

    setlist_ptr = (CTRLSETLIST_OBJ_T *)base_ctrl_ptr;

    //label is exist border
    if (GUI_BORDER_NONE == setlist_ptr->default_style.border.type)
    {
        is_border = FALSE;
    }

    //get font height
    font_height = GUI_GetFontHeight(setlist_ptr->default_style.font_type, UNICODE_HANZI);

    line_height = (uint16)(font_height + 4);
    
    //add border
    if (is_border)
    {
        line_height = (uint16)(line_height + (setlist_ptr->default_style.border.width*2));
    }
    
    if (setlist_ptr->is_need_arrow)
    {
        GUIRES_GetImgWidthHeight(&image_width, &image_height, 
                setlist_ptr->default_style.left_arrow_icon_up, setlist_ptr->win_id);

        *height_ptr = MAX(line_height, image_height + setlist_ptr->default_style.border.width*2);
    }
    else
    {
        *height_ptr = line_height;
    }


    GUI_INVALID_PARAM(width);/*lint !e522*/

    return (result);
}

/*****************************************************************************/
// Description : 设置显示的字符串
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDispTextInfo(
    CTRLSETLIST_OBJ_T *setlist_ctrl_ptr,
    MMI_STRING_T *text_ptr
)
{
    uint32 buf_size = 0;
    
    if (PNULL == setlist_ctrl_ptr || PNULL == text_ptr)
    {
        return FALSE;
    }

    ReleaseMultiTextInfo(setlist_ctrl_ptr);
    
    buf_size = text_ptr->wstr_len * sizeof(wchar);
    setlist_ctrl_ptr->disp_str.wstr_len = text_ptr->wstr_len;
    setlist_ctrl_ptr->disp_str.wstr_ptr = SCI_ALLOC_APP(buf_size);

    SCI_MEMSET(setlist_ctrl_ptr->disp_str.wstr_ptr, 0, buf_size);

    SCI_MEMCPY(setlist_ctrl_ptr->disp_str.wstr_ptr,
        text_ptr->wstr_ptr,
        buf_size);

    return TRUE;
}

/*****************************************************************************/
// Description : Set the back ground color.
// Global resource dependence : 
// Author: 
// Note:设置控件的背景,FALSE,设置错误,TRUE设置成功
/*****************************************************************************/
LOCAL BOOLEAN SetBackGround(
                            CTRLBASE_OBJ_T  *base_ctrl_ptr, 
                            GUI_BG_T    *bg_ptr
                            )
{
    BOOLEAN result = FALSE;
    CTRLSETLIST_OBJ_T *setlist_ptr = (CTRLSETLIST_OBJ_T*)base_ctrl_ptr;

    if (PNULL != bg_ptr && PNULL != setlist_ptr)
    {
        result = TRUE;

        setlist_ptr->default_style.bg_info = *bg_ptr;
    }

    return result;
}

/*****************************************************************************/
// Description : set control font info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFont(
                      CTRLBASE_OBJ_T      *base_ctrl_ptr,
                      GUI_FONT_ALL_T  *font_ptr
                      )
{
    BOOLEAN result = FALSE;
    CTRLSETLIST_OBJ_T *setlist_ptr = (CTRLSETLIST_OBJ_T*)base_ctrl_ptr;

    if (PNULL != font_ptr && PNULL != setlist_ptr)
    {
        result = TRUE;
        setlist_ptr->default_style.font_color = font_ptr->color;
        setlist_ptr->default_style.font_type = font_ptr->font;
    }

    return result;
}

/*****************************************************************************/
// Description : get control font info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFont(
                      CTRLBASE_OBJ_T      *base_ctrl_ptr,
                      GUI_FONT_ALL_T  *font_ptr
                      )
{
    BOOLEAN result = FALSE;
    CTRLSETLIST_OBJ_T *setlist_ptr = (CTRLSETLIST_OBJ_T*)base_ctrl_ptr;

    if (PNULL != font_ptr && PNULL != setlist_ptr)
    {
        result = TRUE;
        
        font_ptr->color = setlist_ptr->default_style.font_color;
        font_ptr->font  = setlist_ptr->default_style.font_type;
    }

    return result;
}

/*****************************************************************************/
// Description : setlist的消息处理
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E GUISETLIST_HandleMessage(
                                            CTRLBASE_OBJ_T        *ctrl_ptr, //控件的指针
                                            MMI_MESSAGE_ID_E  msg_id,   //事件类型
                                            DPARAM            param     //附带的处理参数
                                            )
{
#ifdef TOUCH_PANEL_SUPPORT //xiyuan edit for pclint
    GUI_POINT_T         point           = {0};
#endif
    MMI_RESULT_E        result          = MMI_RESULT_TRUE;
    CTRLSETLIST_OBJ_T   *setlist_ptr  = PNULL;
    
    setlist_ptr = (CTRLSETLIST_OBJ_T*)ctrl_ptr;

    // 参数检查
    
    if (!TypeOfSetList(ctrl_ptr))
    {
        //SCI_TRACE_LOW:"GUISETLIST_HandleMessage error ctrl type!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_3112_112_2_18_3_21_23_307,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    switch( msg_id )
    {
    case MSG_CTL_OPEN: // 打开控件
        break;
        
    case MSG_CTL_GET_ACTIVE:
        GUICTRL_SendNotify(ctrl_ptr->handle,MSG_NOTIFY_GET_ACTIVE);
        break;
        
    case MSG_CTL_LOSE_ACTIVE:
        GUICTRL_SendNotify(ctrl_ptr->handle, MSG_NOTIFY_LOSE_ACTIVE);
        break;
        
    case MSG_CTL_GET_FOCUS: // 得到焦点
        break;

    case MSG_CTL_LOSE_FOCUS: // 失去焦点
        break;
    
    case MSG_CTL_PAINT: // 控件刷新消息
        DrawSetlistItem(setlist_ptr);
        break;

    case MSG_APP_OK:
        if(GetState(setlist_ptr->ctrl_state, GUISETLIST_STATE_LEFTSK_SWITCH))
        {
            HandleSetListWebKeyDown(setlist_ptr);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
            
    case MSG_APP_WEB:
        HandleSetListWebKeyDown(setlist_ptr);
        break;
        
    case MSG_TIMER: // 时钟消息
        result = MMI_RESULT_TRUE;
        break;

    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        result = HandleSetListRightKeyDown(setlist_ptr);
        break;
        
    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        result = HandleSetListLeftKeyDown(setlist_ptr);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        HandleSetListTpDown(setlist_ptr, &point);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        result = MMI_RESULT_FALSE; // 没做处理
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP: // TP UP时响应操作
        HandleSetListTpUp(setlist_ptr, param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CTL_SETLIST_OPENWIN:
        OpenSetlistDispWin(setlist_ptr); 
        break;

    case MSG_CTL_SETLIST_SET:
        {
            uint32 cur_item_index = 0;
            BOOLEAN  is_next = FALSE;
            
            if (PNULL != param)
            {
                cur_item_index = *(uint32*)(param);

                if (GUISETLIST_TYPE_CHECKLIST == setlist_ptr->list_type)
                {
                    GUICTRL_PostNotify(ctrl_ptr->handle, MSG_CTL_SETLIST_SWITCH);
                }
                else
                {
                    if (setlist_ptr->cur_item_index != cur_item_index)
                    {
                        setlist_ptr->cur_item_index = cur_item_index;
                        setlist_ptr->is_disptxt_auto = TRUE;

                        // is_next 在pop窗口时毫无意义，因为此时不是逐个切换
                        GUICTRL_PostNotifyEx(ctrl_ptr->handle,MSG_CTL_SETLIST_SWITCH, &is_next, sizeof(BOOLEAN));
                    }
                }
                
            }
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
// Description : Set the back ground color.
// Global resource dependence : 
// Author: 
// Note:设置控件的背景,FALSE,设置错误,TRUE设置成功
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetBackGround(
                                        MMI_CTRL_ID_T ctrl_id, 
                                        GUI_BG_T *bg_ptr
                                        )
{
    BOOLEAN result = FALSE;
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);

    if (PNULL != bg_ptr && PNULL != setlist_ptr)
    {
        result = TRUE;
        setlist_ptr->default_style.bg_info.bg_type = bg_ptr->bg_type;
        setlist_ptr->default_style.bg_info.img_id = bg_ptr->img_id;
        setlist_ptr->default_style.bg_info.color = bg_ptr->color;
    }

    return result;
}

/*****************************************************************************/
// Description : 增加一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN:  返回当前item的总数
// Note:    
/*****************************************************************************/
PUBLIC uint32 CTRLSETLIST_AddItem(
                                 MMI_CTRL_ID_T ctrl_id, 
                                 const GUISETLIST_ITEM_INFO_T *item_info_ptr
                                 )
{
    uint32 item_num = 0;
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);

    if (PNULL != item_info_ptr && PNULL != setlist_ptr)
    {
        item_num = AddItem(setlist_ptr, item_info_ptr);
    }

    return item_num;
}

/*****************************************************************************/
// Description : 根据text的id增加一个Item
// Global resource dependence : 
// Author: jinwang
// RETRUN:  返回当前item的总数
// Note:    
/*****************************************************************************/
PUBLIC uint32 CTRLSETLIST_AddItemById(
                                 MMI_CTRL_ID_T ctrl_id, 
                                 MMI_TEXT_ID_T text_id
                                 )
{
    uint32 item_num = 0;
    GUISETLIST_ITEM_INFO_T item_info = {0};
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);

    if(PNULL != setlist_ptr)
    {
        MMITHEME_GetResText(text_id, setlist_ptr->win_id, &item_info.str_info);
        if(PNULL != item_info.str_info.wstr_ptr)
        {
            item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
            item_num = AddItem(setlist_ptr, &item_info);
        }
    }

    return item_num;
}
/*****************************************************************************/
// Description : 增加一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN:  返回当前item的总数
// Note:    
/*****************************************************************************/
PUBLIC void CTRLSETLIST_CallBack(
                                MMI_CTRL_ID_T ctrl_id,
                                IGUICTRL_T    *list_ctrl_ptr,
                                DPARAM        param, // 这个参数可以是任意类型的指针，根据不同的情况进行解析
                                MMI_SETLIST_CALL_TYPE_E call_type
                                )
{
    uint16 index = 0;
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);
	CTRLBASE_OBJ_T *   ctrl_ptr = (CTRLBASE_OBJ_T * )setlist_ptr;
    MMI_HANDLE_T list_handle = 0;
    uint16 list_num = 0;

    if (PNULL != list_ctrl_ptr)
    {
        list_handle = GUICTRL_GetCtrlHandle(list_ctrl_ptr);
        list_num = GUILIST_GetTotalItemNum(list_handle);
    }

    if (PNULL != setlist_ptr)
    {
        switch(call_type)
        {
        case MMI_SETLIST_ALLOC:
            if (0 != list_handle)
            {
                GUILIST_SetMaxItem(list_handle, setlist_ptr->item_info.item_total_num, FALSE);
            }
            break;

        case MMI_SETLIST_APPEND_ITEM:
            if (PNULL != list_ctrl_ptr)
            {
                BOOLEAN is_full_win = FALSE;
                
                is_full_win = GetState(setlist_ptr->ctrl_state, GUISETLIST_STATE_OPEN_FULLWIN);
                    
                for (index = 0; index < setlist_ptr->item_info.item_total_num; index++)
                {
                    GUILIST_ITEM_T list_item = {0};
                    GUILIST_ITEM_DATA_T item_data   = {0};
                    MMI_LINK_NODE_T     *tmp_node_ptr   = PNULL;
                    GUISETLIST_ITEM_INFO_T *item_info_ptr = PNULL;
                    
                    SCI_MEMSET(&list_item,0,sizeof(GUILIST_ITEM_T));
                    
                    // 判断是否全屏窗口
                    if (is_full_win)
                    {
                        list_item.item_style = GUIITEM_STYLE_ONE_LINE_CHECK;
                    }
                    else
                    {
                        list_item.item_style = GUIITEM_STYLE_ONE_LINE_CHECK_EXT;
                    }

                    list_item.item_data_ptr = &item_data;
                    
                    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                    
                    tmp_node_ptr = SearchItemByIndex(setlist_ptr, index);
                    
                    if (PNULL != tmp_node_ptr && (PNULL != tmp_node_ptr->data))
                    {
                        item_info_ptr = (GUISETLIST_ITEM_INFO_T *)tmp_node_ptr->data;
                        item_data.item_content[0].item_data.text_buffer.wstr_len = item_info_ptr->str_info.wstr_len;
                        
                        item_data.item_content[0].item_data.text_buffer.wstr_ptr = item_info_ptr->str_info.wstr_ptr;

                        if (GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_INVISIBLE))
                        {
                            list_item.item_state |= GUIITEM_STATE_GRAY;
                        }

                        GUILIST_AppendItem(list_handle, &list_item);
                        GUILIST_SetSelectedItem(list_handle, index, item_info_ptr->is_selected);
                    }
                }
            }
            break;

        case MMI_SETLIST_SET_ITEM_STYLE:
            if (PNULL != list_ctrl_ptr && PNULL != param)
            {
                //SCI_TRACE_LOW:"GUISETLIST_CallBack MMI_SETLIST_SET_ITEM_STYLE %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_3385_112_2_18_3_21_24_308,(uint8*)"d", *(uint32 *)param);
                if (0 == *(uint32 *)param)
                {
                    for (index = 0; index < list_num; index++)
                    {
                        GUILIST_SetItemStyle(list_handle, GUIITEM_STYLE_ONE_LINE_CHECK_EXT, index);
                    }
                }
                else
                {
                    for (index = 0; index < list_num; index++)
                    {
                        GUILIST_SetItemStyle(list_handle, GUIITEM_STYLE_ONE_LINE_CHECK, index);
                    }
                }
            }
            break;

        case MMI_SETLIST_GET_CUR_RECT:
            // 计算list显示区域
            if (PNULL != param)
            {
                GUI_RECT_T *rect_ptr = (GUI_RECT_T *)param;
                *rect_ptr = ctrl_ptr->rect;
            }
            break;

        case MMI_SETLIST_GET_TITLE_STRLEN:
            if (PNULL != param)
            {
                *(uint32*)param = setlist_ptr->title_str_info.wstr_len;
            }
            break;

        case MMI_SETLIST_GET_TITLE_STRDATA:
            if (PNULL != param)
            {
                MMI_STRING_T *str_ptr = PNULL;
                uint16 len = 0;

                str_ptr = (MMI_STRING_T*)param;

                len = MIN(str_ptr->wstr_len, setlist_ptr->title_str_info.wstr_len);

                SCI_MEMCPY(str_ptr->wstr_ptr, setlist_ptr->title_str_info.wstr_ptr, sizeof(wchar) * len);
            }
            break;

        case MMI_SETLIST_IS_OPEN_FULL_WIN:
            if (PNULL != param)
            {
                *(BOOLEAN*)param = GetState(setlist_ptr->ctrl_state, GUISETLIST_STATE_OPEN_FULLWIN);
                //SCI_TRACE_LOW:"GUISETLIST_CallBack MMI_SETLIST_IS_OPEN_FULL_WIN %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUISETLIST_3460_112_2_18_3_21_24_309,(uint8*)"d", *(BOOLEAN *)param);
            }
            break;
            
        default:
            break;
        }
    }

    return;
}


/*****************************************************************************/
// Description : 插入一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN:  返回插入的item索引号
// Note:    
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_InsertItem(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     const GUISETLIST_ITEM_INFO_T *item_info_ptr,
                                     uint32 item_index
                                     )
{
    BOOLEAN result = FALSE;
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);

    if (PNULL != item_info_ptr && PNULL != setlist_ptr)
    {
        if (InsertItem(setlist_ptr, item_info_ptr, item_index) != GUISETLIST_INVALID_ITEM_INDEX)
        {
            result = TRUE;
        }
    }

    return result;
}
/*****************************************************************************/
// Description : 修改一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN: 
// Note:    
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_ModifyItem(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     const GUISETLIST_ITEM_INFO_T *item_info_ptr,
                                     uint32 item_index
                                     )
{
    BOOLEAN result = FALSE;
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);
    
    if (PNULL != item_info_ptr && PNULL != setlist_ptr)
    {
        result = ModifyItem(setlist_ptr, item_info_ptr, item_index);
    }
    
    return result;
}

/*****************************************************************************/
// Description : 增加一个Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN:  返回当前item的总数
// Note:    
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_DeleteItem(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     uint32 item_index
                                     )
{
    BOOLEAN result = FALSE;
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);

    if (PNULL != setlist_ptr)
    {
        result = DeleteItem(setlist_ptr, item_index);
    }

    return result;
}


/*****************************************************************************/
// Description : 删除所有Item
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN: 
// Note:    
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_DeleteAllItem(
				MMI_CTRL_ID_T ctrl_id
                         )
{
	CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);
	return DeleteAllItem(setlist_ptr);
}

/*****************************************************************************/
// Description : 设置当前索引号
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN: 如果设置的item index对应的不存在,或者存在了但是不可见,返回FALSE
// Note:    
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetCurIndex(
                                      MMI_CTRL_ID_T ctrl_id, 
                                      uint32 item_index
                                      )
{
    BOOLEAN                 result          = FALSE;
    GUISETLIST_ITEM_INFO_T  *item_info_ptr  = PNULL;
    CTRLSETLIST_OBJ_T       *setlist_ptr    = GetSetListPtr(ctrl_id);
    MMI_LINK_NODE_T         *tmp_node_ptr   = PNULL;

    if (PNULL != setlist_ptr)
    {
        // 查找
        tmp_node_ptr = SearchItemByIndex(setlist_ptr, item_index);

        // 检查
        if (PNULL != tmp_node_ptr && (PNULL != tmp_node_ptr->data))
        {
            item_info_ptr = (GUISETLIST_ITEM_INFO_T *)tmp_node_ptr->data;
            
            if (!GetState(item_info_ptr->item_state, GUISETLIST_ITEM_STATE_INVISIBLE))
            {
                setlist_ptr->cur_item_index = item_index;
                
                result = TRUE;
            }
        }
    }

    return result;
}

/*****************************************************************************/
// Description : 获取当前索引,如果当前总数为0,返回0xffffffff
// Global resource dependence : 
// Author: xiaoqing.lu
// RETRUN:  如果总数为0,返回GUISETLIST_INVALID_ITEM_INDEX
// Note:    
/*****************************************************************************/
PUBLIC uint32 CTRLSETLIST_GetCurIndex(
                                     MMI_CTRL_ID_T ctrl_id
                                     )
{
    uint32              cur_index       = GUISETLIST_INVALID_ITEM_INDEX;
    CTRLSETLIST_OBJ_T   *setlist_ptr    = GetSetListPtr(ctrl_id);

    if (PNULL != setlist_ptr && setlist_ptr->item_info.item_total_num > 0)
    {
        cur_index = setlist_ptr->cur_item_index;
    }

    return cur_index;
}

/*****************************************************************************/
// Description : 动态创建控件
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_CreateCtrlDyn(
                                        MMI_HANDLE_T win_handle,
                                        MMI_CTRL_ID_T ctrl_id,
                                        GUI_BOTH_RECT_T *rect_ptr
                                        )
{
    BOOLEAN result = FALSE;
    MMI_CONTROL_CREATE_T     create    = {0};
    GUISETLIST_INIT_DATA_T   init_data = {0};
    
    if (PNULL != rect_ptr)
    {
        init_data.both_rect = *rect_ptr;
    }
    
    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_SETLIST_ID;
    create.parent_win_handle = win_handle;
    create.init_data_ptr = &init_data;
    
    if (PNULL != MMK_CreateControl( &create ))
    {
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// Description : set control display rect
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetDisplayRect(
                                         MMI_CTRL_ID_T ctrl_id,  //in:
                                         const GUI_RECT_T *rect_ptr,      //in:
                                         BOOLEAN          is_update       //in:
                                         )
{
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);

    return SetDisplayRect((CTRLBASE_OBJ_T*)setlist_ptr, rect_ptr, is_update);
}

/*****************************************************************************/
// Description : get control display rect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_GetDisplayRect(
                                         MMI_CTRL_ID_T ctrl_id,  //in:
                                         GUI_RECT_T   *rect_ptr       //in/out:
                                         )
{
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);

    return GetDisplayRect((CTRLBASE_OBJ_T*)setlist_ptr, rect_ptr);
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// Description : set setting list tp rect
// Global resource dependence : 
// Author: Jassmine 
// Note: only for form
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetTpRect(
                                    MMI_CTRL_ID_T   ctrl_id,    //in:
                                    GUI_RECT_T      *rect_ptr   //in:
                                    )
{
    BOOLEAN             result = FALSE;
    CTRLSETLIST_OBJ_T   *setlist_ctrl_ptr = PNULL;

    //get setting list pointer by control id
    setlist_ctrl_ptr = GetSetListPtr(ctrl_id);
    if ((PNULL != setlist_ctrl_ptr) &&
        (PNULL != rect_ptr))
    {
        setlist_ctrl_ptr->tp_rect = *rect_ptr;
        result = TRUE;
    }

    return (result);
}
#endif

/*****************************************************************************/
// Description : set control boder info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetBorder(
                                    MMI_CTRL_ID_T ctrl_id,  //in:
                                    GUI_BORDER_T  *border_ptr
                                    )
{
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);

    return SetBorder((CTRLBASE_OBJ_T*)setlist_ptr, border_ptr);
}

/*****************************************************************************/
// Description : set control boder info
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetFont(
                                  MMI_CTRL_ID_T ctrl_id,  //in:
                                  GUI_FONT_ALL_T  *font_ptr
                                  )
{
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);

    return SetFont((CTRLBASE_OBJ_T*)setlist_ptr, font_ptr);
}

/*****************************************************************************/
// Description : 设置文字显示排版
// Global resource dependence : 
// Author: xiaoqing.lu
// Note: default : ALIGN_HVMIDDLE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetFontAlign(
                                       MMI_CTRL_ID_T ctrl_id,  //in:
                                       GUI_ALIGN_E font_align
                                       )
{
    BOOLEAN result = FALSE;
    CTRLSETLIST_OBJ_T *setlist_ptr = GetSetListPtr(ctrl_id);

    if (PNULL != setlist_ptr)
    {
        setlist_ptr->default_style.align = font_align;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// Description : set setting list is visible
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetVisible(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     BOOLEAN        is_visible,
                                     BOOLEAN        is_update
                                     )
{
    BOOLEAN             result = FALSE;
    CTRLSETLIST_OBJ_T  *setlist_ptr = PNULL;
    CTRLBASE_OBJ_T     *ctrl_ptr = PNULL;
    
	//get setting list control pointer
    setlist_ptr = GetSetListPtr(ctrl_id);
    if (PNULL != setlist_ptr)
    {
        ctrl_ptr = (CTRLBASE_OBJ_T *)setlist_ptr;
        //set visible
        IGUICTRL_SetState((IGUICTRL_T *)setlist_ptr,GUICTRL_STATE_INVISIBLE,(BOOLEAN)(!is_visible));

        if (is_update)
        {
            if (is_visible)
            {
                //display
                DrawSetlistItem(setlist_ptr);
            }
            else
            {
                //notify parent update bg
                GUICTRL_SendNotifyEx(ctrl_ptr->handle,MSG_NOTIFY_UPDATE,&ctrl_ptr->rect);
            }
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : 设置状态
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void CTRLSETLIST_SetCtrlState(
                                    MMI_CTRL_ID_T     ctrl_id, 
                                    uint32 aim_state,
                                    BOOLEAN is_set
                                    )
{
    CTRLSETLIST_OBJ_T  *setlist_ptr = PNULL;

    //get setting list control pointer
    setlist_ptr = GetSetListPtr(ctrl_id);

    SetState(&setlist_ptr->ctrl_state, aim_state, is_set);
}

/*****************************************************************************/
// Description : set setting list is circular handle
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetCircular(
                                      MMI_CTRL_ID_T     ctrl_id,
                                      BOOLEAN           is_circular
                                      )
{
    BOOLEAN             result = FALSE;
    CTRLSETLIST_OBJ_T  *setlist_ptr = PNULL;

    //get setting list control pointer
    setlist_ptr = GetSetListPtr(ctrl_id);
    if (PNULL != setlist_ptr)
    {
        result = SetCircularHandleLeftRight((CTRLBASE_OBJ_T *)setlist_ptr,is_circular);
    }

    return (result);
}

/*****************************************************************************/
// Description : set setting list left/right arrow icon
// Global resource dependence : 
// Author: Jassmine
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetArrow(
                                   MMI_CTRL_ID_T     ctrl_id,
                                   MMI_IMAGE_ID_T   left_down,
                                   MMI_IMAGE_ID_T   left_up,
                                   MMI_IMAGE_ID_T   right_down,
                                   MMI_IMAGE_ID_T   right_up
                                   )
{
    BOOLEAN             result = FALSE;
    CTRLSETLIST_OBJ_T  *setlist_ptr = PNULL;

    //get setting list control pointer
    setlist_ptr = GetSetListPtr(ctrl_id);
    if (PNULL != setlist_ptr)
    {
        setlist_ptr->default_style.left_arrow_icon_down  = left_down;
        setlist_ptr->default_style.left_arrow_icon_up    = left_up;
        setlist_ptr->default_style.right_arrow_icon_down = right_down;
        setlist_ptr->default_style.right_arrow_icon_up   = right_up;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : set setting list need arrow
// Global resource dependence : 
// Author: Xiaoqing.Lu
// Note: default FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetNeedArrow(
                                       MMI_CTRL_ID_T     ctrl_id,
                                       BOOLEAN is_need_arrow
                                       )
{
    BOOLEAN             result = FALSE;
    CTRLSETLIST_OBJ_T  *setlist_ptr = PNULL;

    //get setting list control pointer
    setlist_ptr = GetSetListPtr(ctrl_id);
    if (PNULL != setlist_ptr)
    {
        setlist_ptr->is_need_arrow = is_need_arrow;
        result = TRUE;
    }

    return (result);
}


/*****************************************************************************/
// Description : set setting list string effect
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetStrEffect(
                                       MMI_CTRL_ID_T    ctrl_id,
                                       GUIFONT_EFFECT_T effect
                                       )
{
    BOOLEAN             result = FALSE;
    CTRLSETLIST_OBJ_T   *setlist_ctrl_ptr = PNULL;

    //get setting list control pointer
    setlist_ctrl_ptr = GetSetListPtr(ctrl_id);
    if (PNULL != setlist_ctrl_ptr)
    {
        setlist_ctrl_ptr->default_style.str_state = setlist_ctrl_ptr->default_style.str_state | GUISTR_STATE_EFFECT;
        setlist_ctrl_ptr->default_style.effect    = effect;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// Description : set setting list style
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetStyleById(
    MMI_CTRL_ID_T ctrl_id, 
    GUISETLIST_TYPE_E type
)
{
    CTRLSETLIST_OBJ_T *setlist_ctrl_ptr = PNULL;

    //get setting list control pointer
    setlist_ctrl_ptr = GetSetListPtr(ctrl_id);
    
    if (PNULL != setlist_ctrl_ptr)
    {
        setlist_ctrl_ptr->list_type = type;

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// Description : 获取item状态
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_GetItemState(
    MMI_CTRL_ID_T ctrl_id,
    uint32 item_index
)
{
    CTRLSETLIST_OBJ_T *setlist_ctrl_ptr = PNULL;
    
    //get setting list control pointer
    setlist_ctrl_ptr = GetSetListPtr(ctrl_id);
    
    if (PNULL == setlist_ctrl_ptr)
    {
        return FALSE;
    }

    return GetItemState(setlist_ctrl_ptr, item_index);
}

/*****************************************************************************/
// Description : 重置所有item状态
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_ResetAllItemState(MMI_CTRL_ID_T ctrl_id)
{
    CTRLSETLIST_OBJ_T *setlist_ctrl_ptr = PNULL;
    uint16 item_idx = 0;
    
    //get setting list control pointer
    setlist_ctrl_ptr = GetSetListPtr(ctrl_id);
    
    if (PNULL == setlist_ctrl_ptr)
    {
        return FALSE;
    }

    for (item_idx = 0; item_idx < setlist_ctrl_ptr->item_info.item_total_num; item_idx++)
    {
        SetItemState(setlist_ctrl_ptr, item_idx, FALSE);
    }
    
    return TRUE;
}

/*****************************************************************************/
// Description : 设置item状态
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetItemState(
    MMI_CTRL_ID_T ctrl_id,
    uint32 item_index,
    BOOLEAN item_state
)
{
    CTRLSETLIST_OBJ_T *setlist_ctrl_ptr = PNULL;
    
    //get setting list control pointer
    setlist_ctrl_ptr = GetSetListPtr(ctrl_id);
    
    if (PNULL == setlist_ctrl_ptr)
    {
        return FALSE;
    }
    
    SetItemState(setlist_ctrl_ptr, item_index, item_state);
    
    return TRUE;
}

/*****************************************************************************/
// Description : 设置显示的字符串
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetDispText(
    MMI_CTRL_ID_T ctrl_id, 
    MMI_STRING_T *str_ptr,
    BOOLEAN is_need_update
)
{
    CTRLSETLIST_OBJ_T *setlist_ctrl_ptr = PNULL;

    //get setting list control pointer
    setlist_ctrl_ptr = GetSetListPtr(ctrl_id);
    
    if (PNULL == setlist_ctrl_ptr)
    {
        return FALSE;
    }

    SetDispTextInfo(setlist_ctrl_ptr, str_ptr);
    setlist_ctrl_ptr->is_disptxt_auto = FALSE;

    if (is_need_update)
    {
        DrawSetlistItem(setlist_ctrl_ptr);
    }

    return TRUE;
}

/*****************************************************************************/
// Description : set multi selected item text
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSETLIST_SetDefaultText(MMI_CTRL_ID_T ctrl_id)
{
    CTRLSETLIST_OBJ_T *setlist_ctrl_ptr = PNULL;

    //get setting list control pointer
    setlist_ctrl_ptr = GetSetListPtr(ctrl_id);
    
    if (PNULL == setlist_ctrl_ptr)
    {
        return FALSE;
    }

    SetMultiDispText(setlist_ctrl_ptr);

    return TRUE;
}

/*****************************************************************************/
// Description : 获取选中item的index
// Global resource dependence : 
// Author: jin.wang
// Note:
/*****************************************************************************/
PUBLIC uint16 CTRLSETLIST_GetSelectItemIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16* idx_ptr,
    uint16 array_len
)
{
    uint16 selected_cnt = 0; 
    CTRLSETLIST_OBJ_T *list_ptr = GetSetListPtr(ctrl_id);
    
    if(PNULL == list_ptr)
    {
        return selected_cnt;
    }

    selected_cnt = GetSelectedItem(list_ptr, array_len, idx_ptr);                 
    
    return selected_cnt;    
}

