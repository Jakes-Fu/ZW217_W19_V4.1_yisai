/*****************************************************************************
** File Name:      mmimenu_synchronize.h                                      *
** Author:                                                                   *
** Date:           05/20/2011                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2011      jin.wang              Creat
******************************************************************************/
#ifndef _MMIMENU_SYNCHRONIZE_H_
#define _MMIMENU_SYNCHRONIZE_H_ 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guimenu.h"
#include "graphics_draw.h"
#include "mmitheme_mainmenu.h" 
#include "mmi_mainmenu_data.h"
#ifdef DYNAMIC_MAINMENU_SUPPORT

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUIMENU_MAINMENU_STORE_STYLE_NUM ((uint16)GUIMENU_DEVICE_MAX - GUIMENU_DEVICE_SYSTEM)  //有几种存储类型
#define DYNAMIC_MAINMENU_DEFAULT_PAGE_INDEX 0xffff
#define DYNAMIC_MAINMENU_DEFAULT_ITEM_INDEX 0xffff
#define DATA_DIR_NAME "mainmenu"
#define DATA_FILE_FIX_PARA1 ".pr1"
#define DATA_FILE_FIX_PARA2 ".pr2"
#define DATA_FILE_FIX_ICON_ORI ".ori"
#define DATA_FILE_FIX_ICON_DEC ".dec"
#define DATA_FILE_FIX_CHECK_INFO "check_info"
#define DATA_FILE_FIX_COUNT "count"
#define DATA_FILE_FIX_DATA "data"
#define DYNAMIC_DEV_SPACE_LIMIT (32000)
#define DATA_FILE_SIZE_CHECK_INFO (32)
#define DATA_FILE_SIZE_COUNT (32)
//理论上目前平台支持不了100个，图层无法承载
#define DYNAMIC_MAINMENU_NUM_LIMIT (100)

#define MMIMENU_ICON_BORDER_RESERVE 4

//某个盘符下动态菜单数据的总大小，实际应该远远小于此值
#define DATA_FILE_SIZE_LIMIT (999999)

#define ICON_DEC_WIDTH (MENUICON_WIDTH-MMIMENU_ICON_BORDER_RESERVE)

#define ICON_DEC_HEIGHT (MENUICON_HEIGHT-MMIMENU_ICON_BORDER_RESERVE)

//根据图片解码所需空间大小
#define ICON_FILE_SIZE (ICON_DEC_WIDTH * ICON_DEC_HEIGHT * 4)

//启动参数空间限制
#define RUNDATA_FILE_SIZE ICON_FILE_SIZE_LIMIT

//根据图片解码所需空间设定，事实上远远小于这个数值
#define ICON_FILE_SIZE_LIMIT (MENUICON_HEIGHT * MENUICON_WIDTH * 4)
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
enum
{
    GUIMENU_READFILE_NONE_STYLE = 0,   /*!< 无 */
    GUIMENU_READFILE_DECODE = 1,       /*!< 读取解码图片 */
    GUIMENU_READFILE_ORIG = 2,         /*!< 读取原始图片 */
    GUIMENU_READFILE_BOTH = 1|2,       /*!< 解码、原始都读取 */
};
typedef uint32 GUIMENU_READFILE_TYPE;

enum
{
    GUIMENU_GET_INFO_NONE = 0,   /*!< 不读取 */
    GUIMENU_GET_INFO_PARA = 1,   /*!< 读取参数文件 */
    GUIMENU_GET_INFO_ICON = 2,   /*!< 读取图标文件 */
    GUIMENU_GET_INFO_ALL = 1|2,  /*!< 读取参数、图标文件 */
};
typedef uint32 GUIMENU_GET_INFO_T;

/*! @struct  MMIMENU_SYN_VALUE_E
@brief 所有动态菜单返回值类型
*/
typedef enum
{
	MMIMENU_SYN_VALUE_SUC,
	MMIMENU_SYN_VALUE_NOSPACE,	
	MMIMENU_SYN_VALUE_INVALID,
	MMIMENU_SYN_VALUE_EXIST,
	MMIMENU_SYN_VALUE_ERROR,

	MMIMENU_SYN_VALUE_MAX// 系统保留
}MMIMENU_SYN_VALUE_E;

/*! @struct  MMIMENU_SYN_HEAD_T
@brief 所有动态菜单在文件系统中的存储格式
*/
typedef struct
{
    uint32 ver_info;                                    /*!< 系统版本信息，用于检查数据合法性 */
    uint32 check_info;                                  /*!< 动态菜单判断是否需要同步*/
    uint32 dy_menu_num;                                 /*!< 动态菜单个数 */
} MMIMENU_SYN_HEAD_T;

// 图片缩放缓存数据
typedef struct
{
    uint8               *buf_ptr;
    uint8               *target_buf_ptr;
    uint16              width;
    uint16              height;
} MMIMENU_SYN_ICON_T;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief 增加一个动态menu
@author jin.wang
@param dynamic_item_ptr [in] 新增item的数据
@param store_idx [in] 安装盘符
@return 操作是否成功
@note
- dynamic_item_ptr中，以下不需要提供
    -# uint16 page_index
    -# uint16 item_index
    -# MMI_MENU_ID_T menu_id
- dynamic_item_ptr中的dynamic_menu_info需要填写完整
@sa MMIMENU_DyDeleteItem
@sa MMIMENU_DyDeleteItemByFunc
*******************************************************************************/
PUBLIC MMIMENU_SYN_VALUE_E MMIMENU_DyAppendItem(
												GUIMAINMENU_DYNAMIC_INFO_T *dynamic_item_ptr,
												GUIMENU_FILE_DEVICE_E store_idx
												);

/***************************************************************************//*!
@brief 删除一个动态item
@author jin.wang
@param dynamic_item_ptr [in] 欲删除item的数据
@return 操作是否成功
@note 事实上是根据显示字符串和启动函数
- dynamic_item_ptr中，仅需要提供dynamic_menu_info中的部分数据
    -# wchar text[GUIMENU_DYMAINMENU_STR_MAX_NUM + 1]
    -# DY_MENU_ITEM_LINK_FUNC_T link_function_ptr
@sa MMIMENU_DyAppendItem
@sa MMIMENU_DyDeleteItemByFunc
*******************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyDeleteItem(
    GUIMAINMENU_DYNAMIC_INFO_T *dynamic_item_ptr
);

/***************************************************************************//*!
@brief 删除统一启动函数的所有菜单
@author jin.wang
@param link_func_ptr [in] 启动函数的指针
@return 操作是否成功
@note 会有一类的应用采用相同的启动函数，比如java，此时统一删除菜单
@sa MMIMENU_DyAppendItem
@sa MMIMENU_DyDeleteItem
*******************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyDeleteItemByFunc(
    DY_MENU_ITEM_LINK_FUNC_T link_func_ptr
);

/*****************************************************************************/
// Description : 释放启动参数内存
// Global resource dependence : 
// Author:jin.wang
// Note:返回动态菜单数据信息
/*****************************************************************************/
PUBLIC void MMIMENU_DyFreeParmInfo(
    GUIMAINMENU_DY_ITEM_T *dy_item_ptr
);

/*****************************************************************************/
// Description :动态主菜单初始化
// Global resource dependence : 
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_DyInit(void);

/*****************************************************************************/
// Description :判断动态菜单是否需要同步
// Global resource dependence : 
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyCheckIsSyn(void);

/*****************************************************************************/
// Description :设置动态主菜单位置信息
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_DySaveItemPos(
    uint16                      item_total_num,                         
    BOOLEAN                     is_move_state,                          
    MMIMAINMENU_PAGE_ITEM_INFO_T *page_info_ptr
);

/*****************************************************************************/
// Description : 读取动态主菜单总个数
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC uint32 MMIMENU_DyGetItemCount(void);

/*****************************************************************************/
// Description : 读取所有菜单数据
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyGetAllItemData(void);
/*****************************************************************************/
// Description :  读取动态菜单数据
// Global resource dependence :
// Author: jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DySetItemData(
    MMIMAINMENU_PAGE_ITEM_INFO_T *item_page_ptr,
    MMIMAINMENU_TYPE_E style,
	BOOLEAN is_organize,                                                //是否在组织模式下
	uint16 item_total_num,
    MMI_IMAGE_ID_T select_icon_id
);
/*****************************************************************************/
// Description : 删除一个动态item
// Global resource dependence :
// Author:jin.wang
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyDeleteItemEx(
    CTRLMAINMENU_ITEM_T *dynamic_item_ptr
);

/*****************************************************************************/
// Description : 释放动态菜单数据数组内存
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_DyFreeData(void);

/*****************************************************************************/
// Description : 判断启动函数、参数是否正确
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyIsRunInfoValid(GUIMAINMENU_DYNAMIC_DATA_T *data_ptr);

/*****************************************************************************/
// Description : 根据menu_id获取slide模式下动态菜单运行数据
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DySlideGetRunInfo(
    MMI_MENU_ID_T menu_id,
    GUIMAINMENU_DY_ITEM_T *item_info_ptr
);

/*****************************************************************************/
// Description : 根据list_index获取list模式下动态菜单运行数据
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyListGetRunInfo(
    uint16 list_index,
    GUIMAINMENU_DY_ITEM_T *item_info_ptr
);

/*****************************************************************************/
// Description : 根据menu_id获取slide模式下动态菜单图片数据
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DySlideGetIcon(
    MMI_MENU_ID_T menu_id,
    uint8 *icon_info_ptr
);

/*****************************************************************************/
// Description : 根据menu_id获取slide模式下动态菜单显示数据(不包括图片数据)
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DySlideGetDispInfo(
    MMI_MENU_ID_T menu_id,
    GUIMAINMENU_DY_ITEM_T *item_info_ptr
);

/*****************************************************************************/
// Description : 根据menu_id获取list模式下动态菜单显示数据
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyListGetDispInfo(
    uint16 list_index,
    GUIMAINMENU_DY_ITEM_T *item_info_ptr
);

/*****************************************************************************/
// Description : 根据menu_id获取list模式下动态菜单显示数据(不包括图片数据)
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyListGetDispInfoEx(
    uint16 list_index,
    GUIMAINMENU_DY_ITEM_T *item_info_ptr
);

/*****************************************************************************/
// Description : 判断一个动态菜单数据是否存在
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_IsDyItemExist(
    MMI_MENU_ID_T menu_id
);

/*****************************************************************************/
// Description : 设置高亮图片id
// Global resource dependence :
// Author:jin.wang
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_SetSelectedId(
    MMI_IMAGE_ID_T select_icon_id
);

/*****************************************************************************/
// Description : 设置text组
// Global resource dependence :
// Author:nan.ji
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_DySetItemStringArray(
										 GUIMAINMENU_DYNAMIC_INFO_T *dynamic_item_ptr,
										 GUIMAINMENU_DYNAMIC_STRING_DATA_T *string_arr,
										 uint16 arr_num
										 );

/*****************************************************************************/
// Description : 设置显示用的text
// Global resource dependence :
// Author:nan.ji
// Note:当java和WRE传入不一样时，默认设置显示数据为数据组首个data。
/*****************************************************************************/
PUBLIC void MMIMENU_DySetItemString(
									GUIMAINMENU_DYNAMIC_INFO_T *dynamic_item_ptr
									);

/*****************************************************************************/
// Description : 获取当前语言环境对应的text
// Global resource dependence :
// Author:nan.ji
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_DyGetItemString(
									GUIMAINMENU_DY_ITEM_T *dynamic_item_ptr
									);
/*****************************************************************************/
// Description : 释放动态菜单数据数组内存
// Global resource dependence :
// Author:.nan.ji
// Note:
/*****************************************************************************/
PUBLIC void MMIMENU_DyDestroy(
                                void
                                );
/*****************************************************************************/
// Description : 运行动态menu
// Global resource dependence :
// Author:nan.ji
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_DyRunItem(
                                 MMI_MENU_GROUP_ID_T group_id,
								 MMI_MENU_ID_T menu_id
                                 );                                
#endif

#endif

