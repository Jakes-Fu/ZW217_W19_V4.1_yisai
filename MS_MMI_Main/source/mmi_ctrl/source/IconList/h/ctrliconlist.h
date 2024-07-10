/*****************************************************************************
** File Name:      ctrliconlist.h                                               *
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

#ifndef _CTRLICONLIST_H_
#define _CTRLICONLIST_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "mmi_link.h" 
#include "ctrliconlist_export.h"

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
// iconlist type info
#define CTRL_ICONLIST_TYPE     (ICONLIST_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// iconlist init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;
} CTRLICONLIST_INIT_PARAM_T;

// iconlist class for public or protected method
typedef struct
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;
} CTRLICONLIST_VTBL_T;

#ifdef GUIF_ICONLIST_STUNT
// special type
typedef enum
{
    GUIICONLIST_SPECIAL_NONE = 0,                   // 无
    GUIICONLIST_SPECIAL_FOLDER_TO_FILE,             // 目录到文件
    GUIICONLIST_SPECIAL_FILE_TO_FOLDER,             // 文件到目录
    GUIICONLIST_SPECIAL_FILE_TO_IMAGE,              // 文件到图像
    GUIICONLIST_SPECIAL_IMAGE_TO_FILE,              // 图像到文件
    GUIICONLIST_SPECIAL_MAX
} GUIICONLIST_SPECIAL_E;

// 图像信息
typedef struct
{
    GUIIMG_BITMAP_T         img_bitmap;             // 图像bitmap数据
    MMI_IMAGE_ID_T          img_id;                 // 图像id
} GUIICONLIST_IMG_INFO_T;

// 旋转信息
typedef struct
{
    GUIICONLIST_IMG_INFO_T  	img_info;               // 图像信息
    GUI_RECT_T              	dis_rect;               // 显示位置
    uint16                  	index;                  // 索引号
    uint16                  	rotate_angle;           // 旋转角度
} GUIICONLIST_ROTATE_INFO_T;
#endif

//icon info
typedef struct
{
    BOOLEAN             is_load;    //text是否已经加载
    GUIICONLIST_DISP_T  display;    //icon display info,include name and softkey
} GUIICONLIST_TEXT_T;

//icon info
typedef struct
{
    BOOLEAN     	is_load;    //是否已经加载
    uint32      	ctrl_add;   //控件的地址
} GUIICONLIST_ICON_T;

//item info
typedef struct
{
    BOOLEAN             	is_selected;    //is selected
    GUIICONLIST_TEXT_T  	text;           //text
    GUIICONLIST_ICON_T  	icon;           //icon
    GUIICONLIST_TEXT_T  	text_addtion;   //addtion text
} GUIICONLIST_ITEM_T;

//xiyuan edit
#ifdef GUIF_ICONLIST_DELIMITER
typedef struct  
{
	MMI_LINK_NODE_T         *group_list_ptr;    //group链表
	uint16                  group_num;          //group的数目
}GUIICONLIST_GROUP_LIST_T;

//iconlist group info
typedef struct
{
	uint32          	group_id;                 // group的ID
    uint16          	group_item_total_num;     // 组里所包含的icon的总个数
    uint16          	start_item_index;         // 第一个icon的起始索引值
	MMI_STRING_T    	delimiter_str_info;       // 分隔符内的字符串信息
	GUI_BIG_RECT_T  	group_rect;               // group相对于iconlist的坐标移动
	GUI_BIG_RECT_T  	group_relative_rect;      // group从iconlist display区域的相对的(0，0)区域获取的坐标
} GUIICONLIST_GROUP_INFO_T;
#endif
//edit end
//iconlist control
typedef struct
{
    CTRLBASE_OBJ_T              base_ctrl;
	
    BOOLEAN                 	is_active;          //is active
    BOOLEAN                 	is_markable;        //is markable
    BOOLEAN                 	is_loading_icon;    //is loading icon
    uint8                   	slide_timer_id;     //slide timer id
    BOOLEAN                 	is_tp_down;         //is tp down,may move,not up
    BOOLEAN                 	is_tp_pressed;      //is tp down,not move,not up
	
    uint8                 	    scroll_timer_id;    //scroll timer id
    uint16                  	str_index;          //string start index
	
    uint16                  	selected_max_num;   //allow select max number
    uint16                  	selected_num;       //select number
    
    uint16                  	cur_index;          //current selected icon index,from 0
    uint16                  	total_num;          //the total number of item
    uint16                  	top_index;          //the top item index in current page
    uint16                  	bottom_index;       //the bottom item index in current page
	
    uint16                  	col_num_page;       //column number in one page,all icon are display all
    uint16                  	row_num_page;       //row number in one page,all icon are display all
    uint16                  	icon_num_page;      //icon number in one page,all icon are display all
    uint16                  	row_max_num_page;   //max row number in one page
    uint16                  	icon_cache_num;     //cache icon number
	
    float                  	    fling_velocity;     //fling velocity
    GUI_POINT_T             	start_point;        //slide start point
    GUI_POINT_T                 prev_point;         //slide previous point
	
    MMK_TP_SLIDE_E             	slide_state;        //slide state
    MMK_TP_SLIDE_E             	tpdown_slide_state; //tp down slide state
	
    GUI_BIG_RECT_T              relative_rect;      //relative rect,relative to the icon list with except border,原点是icon list除去边框
    GUI_BIG_RECT_T              relative_disp_rect; //relative display rect,relative to the icon list with except border,原点是icon list除去边框
	
    GUIICONLIST_ITEM_T      	*item_ptr;          //item
	
    IGUICTRL_T		            *prgbox_ctrl_ptr;   //progress box control pointer
    GUIICONLIST_STYLE_E      	style;              //icon list style
    GUIICONLIST_LOAD_E       	load_type;          //icon list load anim type
	
    BOOLEAN                 	is_dynamic_append_mode; //is support dynamic append icon
	
    MMITHEME_ICONLIST_T    		theme;              //theme
	
#ifdef GUIF_ICONLIST_STUNT
    GUIICONLIST_ROTATE_INFO_T   *rotate_info_ptr;   // 保存的旋转信息
    uint16                  	rotate_info_count;      // rotate_info_ptr的个数
	
    GUIICONLIST_SPECIAL_E   	specail_type;           // 动画类型
#endif
	
    MMI_WIN_ID_T            	win_id;
	
    GUIICONLIST_LAYOUT_STYLE_E 	layout_style;        //排版及滑动方式
	
#ifdef GUIF_ICONLIST_DELIMITER
    GUIICONLIST_GROUP_LIST_T 	group_list_info;   //group链表信息
    BOOLEAN                     is_group_rect_dirty; //rect置脏
#endif
    BOOLEAN                 	is_display_title;
    BOOLEAN                 	is_zoom;            //icon是否支持可缩放
} CTRLICONLIST_OBJ_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get icon list pointer by control handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLICONLIST_OBJ_T* ICONLIST_GetPtr(
                                              MMI_HANDLE_T  ctrl_handle
                                              );

/*****************************************************************************/
//  Description : get icon list rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONLIST_GetRect(
                                      CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONLIST_GetScrollBarRect(
                                               CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                                               );

/*****************************************************************************/
//  Description : get icon list content rect,except border,margin and scroll bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONLIST_GetContRect(
                                          CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr
                                          );

/*****************************************************************************/
//  Description : get item width,include icon,font and space
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetItemWidth(
                                       BOOLEAN              is_include_space,   //in:
                                       CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr  //in
                                       );

/*****************************************************************************/
//  Description : get item height,include icon,font and space
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetItemHeight(
                                        BOOLEAN             is_include_space,   //in:
                                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                                        );

/*****************************************************************************/
//  Description : get item horizontal space width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetItemHspace(
                                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                                        );

/*****************************************************************************/
//  Description : get item vertical space width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetItemVspace(
                                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                                        );

/*****************************************************************************/
//  Description : get icon width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetIconWidth(
                                       CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr  //in
                                       );

/*****************************************************************************/
//  Description : get icon height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetIconHeight(
                                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr  //in
                                        );

/*****************************************************************************/
//  Description : get icon rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONLIST_GetIconRect(
										  uint16                index,              //in:
										  CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
										  );

/*****************************************************************************/
//  Description : get icon relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BIG_RECT_T ICONLIST_GetIconRelativeRect(
                                                      uint16                index,              //in:
                                                      CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                                                      );

/*****************************************************************************/
//  Description : get text rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONLIST_GetTextRect(
											  uint16                index,              //in:
											  CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
											  );

/*****************************************************************************/
//  Description : get item rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONLIST_GetItemRect(
										  uint16                index,              //in:
										  CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
										  );

/*****************************************************************************/
//  Description : get item relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BIG_RECT_T ICONLIST_GetItemRelativeRect(
                                                      uint16                index,              //in:
                                                      CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,  //in
													  BOOLEAN               is_include_space
                                                      );

/*****************************************************************************/
//  Description : get icon list relative height,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 ICONLIST_GetRelativeHeight(
                                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                            );

/*****************************************************************************/
//  Description : set icon list display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:   xiyuan edit 2011.12
/*****************************************************************************/
PUBLIC BOOLEAN ICONLIST_SetDispRect(
                                       int32                display_min_position,
                                       CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                                       );

/*****************************************************************************/
//  Description : reset icon list display rect,only for slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 调整显示区域在[0,display_max_top]之间
/*****************************************************************************/
PUBLIC BOOLEAN ICONLIST_ResetDispRect(
                                         CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
                                         );

/*****************************************************************************/
//  Description : get icon list display rect height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetDispHeight(
                                        CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
                                        );

/*****************************************************************************/
//  Description : get icon list display rect max top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 ICONLIST_GetDispMaxTop(
                                       CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
                                       );

#ifdef GUIF_ICONLIST_STUNT
/*****************************************************************************/
//  Description : run file to image animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void ICONLIST_RunFile2Img(
                                    CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                                    );

/*****************************************************************************/
//  Description : run image to file animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void ICONLIST_RunImg2File(
                                    CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                                    );

/*****************************************************************************/
//  Description : run folder to file animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void ICONLIST_RunFolder2File(
                                       CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                                       );

/*****************************************************************************/
//  Description : run file to folder animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void ICONLIST_RunFile2Folder(
                                       CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                                       );
#endif

/*****************************************************************************/
//  Description : get icon list display rect width
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetDispWidth(
									   CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
									   );

/*****************************************************************************/
//  Description : get icon list relative Width,except border
//  Global resource dependence : 
//  Author: xiyuan edit 2011.12
//  Note:
/*****************************************************************************/
PUBLIC uint32 ICONLIST_GetRelativeWidth(
										   CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
										   );

/*****************************************************************************/
//  Description : get icon list display rect max left
//  Global resource dependence : 
//  Author: xiyuan edit 2011.12
//  Note: 
/*****************************************************************************/
PUBLIC int32 ICONLIST_GetDispMaxLeft(
										CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
										);

/*****************************************************************************/
// Description : 设置支持横向滑动，限内部使用
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN ICONLIST_SetLayoutStyleInter(
											   CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
											   GUIICONLIST_LAYOUT_STYLE_E layout_style
											   );

/*****************************************************************************/
// Description : 是否支持横向滑动，限内部使用
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:
/*****************************************************************************/
PUBLIC GUIICONLIST_LAYOUT_STYLE_E ICONLIST_GetLayoutStyleInter(
																  CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr
																  );

/*******************************************************************************************/
//  Description : 通过item所在的行、列得到item的index，限横向滑动使用
//  Global resource dependence :
//  Author: xiyuan.ma 2011.12
//  Note:
/*********************************************************************************************/
PUBLIC uint16 ICONLIST_GetIndexByRowCol(
										   CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr,
										   uint16 row,
										   uint16 col
										   );
										   
#ifdef GUIF_ICONLIST_DELIMITER
/*****************************************************************************/
// Description : 增加一个group结点指针
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:    
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T *ICONLIST_IconListAddGroup(
													 CTRLICONLIST_OBJ_T           *iconlist_ctrl_ptr,
													 GUIICONLIST_DELIMITER_INFO_T *group_ptr
													 );
/*****************************************************************************/
// Description : 获取group的总个数
// Global resource dependence : 
// Author: xiyuan.ma
// Note:    
/*****************************************************************************/
PUBLIC uint32 ICONLIST_GetGroupNum(
									  CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr
									  );

/*****************************************************************************/
// Description : 根据group id在group链表中查找group结点 
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:    
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T *ICONLIST_SearchGroupInListById(
														  CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr,
														  uint32 group_id
														  );

/*****************************************************************************/
// 	Description : 根据item index在group链表中查找group结点 
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:    
/*****************************************************************************/
PUBLIC MMI_LINK_NODE_T *ICONLIST_SearchGroupInListByIndex(
															 CTRLICONLIST_OBJ_T   *iconlist_ctrl_ptr,
															 uint16 item_index
															 );

/*****************************************************************************/
// Description : 获取group的item总个数
// Global resource dependence : 
// Author: xiyuan.ma 2011.12
// Note:    
/*****************************************************************************/
PUBLIC uint16 ICONLIST_GetGroupItemNum(
										  CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
										  MMI_LINK_NODE_T *group_info_ptr
										  );

/*****************************************************************************/
//  Description : 计算每个group在iconlist中的rect,并保存
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
PUBLIC void ICONLIST_GetGroupRect(
									 CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr
									 );

/*****************************************************************************/
// 	Description : 销毁group链表的一个结点
//	Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//	Note:
/*****************************************************************************/
PUBLIC void ICONLIST_DestroyIconListGroupNode(
												 MMI_LINK_NODE_T const *node_ptr
												 );

/*****************************************************************************/
//  Description : 计算item相对于group的坐标
//  Global resource dependence : 
//  Author: xiyuan.ma 2011.12
//  Note:
/*****************************************************************************/
PUBLIC GUI_BIG_RECT_T ICONLIST_GetItemToGroupRect(
													 CTRLICONLIST_OBJ_T *iconlist_ctrl_ptr,
													 uint16             item_index
													 );
#endif
/*****************************************************************************/
//  Description : get iconlist type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T ICONLIST_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
