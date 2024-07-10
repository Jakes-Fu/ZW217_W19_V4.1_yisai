/*****************************************************************************
** File Name:      ctrltitle_export.h                                        *
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

#ifndef _CTRLTITLE_EXPORT_H_
#define _CTRLTITLE_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"

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
/*! @def GUITITLE_TEXT_MAXLEN
@brief title文本最大长度
*/ 
#define GUITITLE_TEXT_MAXLEN 	    60

/*! @def GUITITLE_SUBTEXT_MAXLEN
@brief 索引信息最大长度
*/
#define GUITITLE_SUBTEXT_MAXLEN     15

/*! @typedef GUITITLE_BUTTON_INDEX_T
@brief title的箭头信息
*/
typedef uint8 GUITITLE_BUTTON_INDEX_T;

/*! @def GUITITLE_BUTTON_LEFT
@brief title左边Button
*/
#define GUITITLE_BUTTON_LEFT         0

/*! @def GUITITLE_BUTTON_RIGHT
@brief title右边Button
*/
#define GUITITLE_BUTTON_RIGHT        1

/*! @def GUITITLE_BUTTON_NUM
@brief Button数量
*/
#define GUITITLE_BUTTON_NUM          2

        
/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*!  @typedef GUITITLE_SCROLL_TYPE_E
@brief title scroll
*/
typedef enum
{
    TITLE_NOSCROLL,                 /*!< 无 */
	TITLE_SCROLL_LEFT_OUT,          /*!< 从右向左滚动*/  
	TITLE_NOSCROLL_BY_ELLIPSIS      /*!< 不循环，区域不够则显示省略点 */   
}GUITITLE_SCROLL_TYPE_E;

/*! @typedef GUITITLE_BUTTON_STATE_E
@brief title箭头显示模式
*/
typedef enum
{
    TITLE_BUTTON_NORMAL,     /*!< 正常模式 */
    TITLE_BUTTON_INVISIBLE,  /*!< 不可见模式 */
	TITLE_BUTTON_GRAYED,     /*!< 灰化模式 */
	TITLE_BUTTON_STATE_MAX   /*!< 系统保留 */
}GUITITLE_BUTTON_STATE_E;

typedef enum
{
    GUITITLE_DATA_TEXT_ID,     /*!< 显示文本 */ 
	GUITITLE_DATA_TEXT_BUFFER  /*!< 显示文本 */  
}GUITITLE_TEXT_DATA_E;

typedef struct  
{
    MMI_IMAGE_ID_T       image_id;       /*!< image_id */ 
    MMI_IMAGE_ID_T       press_image_id; /*!< press_image_id */ 
    MMI_IMAGE_ID_T       gray_image_id;  /*!< gray_image_id */ 

    GUITITLE_TEXT_DATA_E text_type;      /*!< 文本类型 */ 
    MMI_TEXT_ID_T        text_id;        /*!< 文本ID */ 
    wchar                *icon_str_ptr;  /*!< 文本指针*/
    uint16               str_len;        /*!< 文本长度 */ 
}GUITITLE_BUTTON_INFO_T;


/*! @struct GUITITLE_INIT_DATA_T
@brief title控件初始化信息
*/
typedef struct
{
  	MMI_TEXT_ID_T		text_id;    /*!< text_id */ 
} GUITITLE_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : 清除sub text的显示信息
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_ClearSubTextInfo(    
                                         MMI_HANDLE_T   win_handle,
                                         MMI_CTRL_ID_T    ctrl_id
                                         );

/*****************************************************************************/
// Description : 动态创建状态栏控件
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_CreateCtrlDyn(
                                      MMI_HANDLE_T win_handle,
                                      MMI_CTRL_ID_T ctrl_id, 
                                      MMI_TEXT_ID_T text_id
                                      );

/*****************************************************************************/
// Description : 获得title上button的状态
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC GUITITLE_BUTTON_STATE_E CTRLTITLE_GetButtonStatus(    
                                                        MMI_HANDLE_T   win_handle,
                                                        MMI_CTRL_ID_T    ctrl_id,
                                                        GUITITLE_BUTTON_INDEX_T   index
                                                        );

/*****************************************************************************/
// Description : 获得title上arrow状态
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC GUITITLE_BUTTON_STATE_E CTRLTITLE_GetRollArrowStatus(    
                                                          MMI_HANDLE_T   win_handle,
                                                          MMI_CTRL_ID_T    ctrl_id,
                                                          GUITITLE_BUTTON_INDEX_T   index
                                                          );

/*****************************************************************************/
// Description : set the title background
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetBackground(
                                      MMI_HANDLE_T   win_handle,
                                      MMI_CTRL_ID_T     ctrl_id,
                                      MMI_IMAGE_ID_T bg_image,
                                      GUI_COLOR_T    bg_color
                                      );

/*****************************************************************************/
// Description : set title font color
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void CTRLTITLE_SetFontColor(
                                  MMI_HANDLE_T      win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  GUI_COLOR_T       font_color
                                  );

/*****************************************************************************/
// Description : 设置title上是否有arrow
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetRollArrow(    
                                     MMI_HANDLE_T   win_handle,
                                     MMI_CTRL_ID_T    ctrl_id,
                                     BOOLEAN        is_need_button  //是否需要title上的arrow图标
                                     );

/*****************************************************************************/
// Description : 设置title上是否有arrow
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetRollArrowStatus(    
                                           MMI_HANDLE_T   win_handle,
                                           MMI_CTRL_ID_T    ctrl_id,
                                           GUITITLE_BUTTON_INDEX_T   index,
                                           GUITITLE_BUTTON_STATE_E  status
                                           );

/*****************************************************************************/
// Description : 设置title上是否有button
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetNeedButton(    
                                      MMI_HANDLE_T   win_handle,
                                      MMI_CTRL_ID_T    ctrl_id,
                                      BOOLEAN        is_need_button  //是否需要title上的arrow图标
                                      );

/*****************************************************************************/
// Description : 设置title上button的状态
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonStatus(    
                                        MMI_HANDLE_T   win_handle,
                                        MMI_CTRL_ID_T    ctrl_id,
                                        GUITITLE_BUTTON_INDEX_T   index,
                                        GUITITLE_BUTTON_STATE_E  status
                                        );

/*****************************************************************************/
// Description : 设置title右下角列表索引信息
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetSubText(    
                                   MMI_HANDLE_T     win_handle,
                                   MMI_CTRL_ID_T    ctrl_id,
                                   BOOLEAN          is_need_sub_text,   //是否需要列表索引信息
                                   const wchar      *wstr_ptr,        //[IN] string pointer
                                   uint16            wstr_len,        //[IN] string length
                                   BOOLEAN          is_need_update        //是否立即显示
                                   );

/*****************************************************************************/
// Description : update the window title
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC void CTRLTITLE_Update(
                            MMI_HANDLE_T    win_handle,
                            MMI_CTRL_ID_T    ctrl_id
                            );

/*****************************************************************************/
// Description : 设置title左边的小图标
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetIcon(    
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                MMI_IMAGE_ID_T  icon_id
                                );

/*****************************************************************************/
// Description : 设置title区域
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetRect(    
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_RECT_T        rect
                                );

/*****************************************************************************/
// Description : 设置title右下角列表索引信息
// Global resource dependence : 
// Author: Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetSubTextParam(
                                        MMI_HANDLE_T    win_handle,
                                        MMI_CTRL_ID_T   ctrl_id,
                                        GUI_RECT_T      *text_rect_ptr,   // 子字符串的区域
                                        GUI_FONT_T      *font_ptr,        // 子字符串字体
                                        GUI_ALIGN_E     *align_ptr        // align type
                                        );
                                        
/*****************************************************************************/
// Description : set the window title string, by text id
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetTextId(
                                  MMI_HANDLE_T        win_handle,
                                  MMI_CTRL_ID_T        ctrl_id,
                                  MMI_TEXT_ID_T        text_id, //[IN] the string id
                                  BOOLEAN            is_need_update //[IN] is flash
                                  );

/*****************************************************************************/
// Description : set the window title align type
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetTextAlignType(
                                         MMI_HANDLE_T                win_handle,
                                         MMI_CTRL_ID_T            ctrl_id,
                                         GUI_ALIGN_E                align
                                         );

/*****************************************************************************/
// Description : set the window title scroll type
// Global resource dependence : 
// Author:Liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetTextScrollType(
                                          MMI_HANDLE_T                win_handle,
                                          MMI_CTRL_ID_T            ctrl_id,
                                          GUITITLE_SCROLL_TYPE_E    scroll_type
                                          );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
