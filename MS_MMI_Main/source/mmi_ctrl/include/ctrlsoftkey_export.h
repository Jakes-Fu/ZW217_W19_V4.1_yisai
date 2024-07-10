/*****************************************************************************
** File Name:      ctrlsoftkey_export.h                                        *
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

#ifndef _CTRLSOFTKEY_EXPORT_H_
#define _CTRLSOFTKEY_EXPORT_H_ 

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
/*! @def BORDER_NULL
@brief 没有边框
*/
#define BORDER_NULL     		0

/*! @def GUISOFTKEY_BUTTON_NUM
@brief softkey一共三个button，左中右
*/
#define GUISOFTKEY_BUTTON_NUM  	3 // softkey一共三个button，左中右

/*! @def LEFT_BUTTON
@brief 左button的索引
*/
#define LEFT_BUTTON     		0 // 左button的索引

/*! @def MIDDLE_BUTTON
@brief 中间button的索引
*/
#define MIDDLE_BUTTON   		1 // 中button的索引

/*! @def RIGHT_BUTTON
@brief 右button的索引
*/
#define RIGHT_BUTTON    		2 // 右button的索引

/*! @def INVALID_BUTTON
@brief 无效button的索引
*/
#define INVALID_BUTTON    		4 // 无效button的索引

/*! @typedef GUI_SOFTKEY_BG_TYPE_T
@brief softkey背景模式信息
*/
typedef uint32  GUI_SOFTKEY_BG_TYPE_T;

/*! @def GUI_SOFTKEY_BG_NULL
@brief 没有背景
*/
#define GUI_SOFTKEY_BG_NULL         0

/*! @def GUI_SOFTKEY_BG_IMAGE_ONLY
@brief 背景只有图片
*/
#define GUI_SOFTKEY_BG_IMAGE_ONLY   1

/*! @def GUI_SOFTKEY_BG_COLOR_ONLY
@brief 背景只有颜色
*/
#define GUI_SOFTKEY_BG_COLOR_ONLY   2

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @def GUISOFTKEY_STYLE_NULL
@brief softkey无风格
*/
#define GUISOFTKEY_STYLE_NULL		0

/*! @typedef GUISOFTKEY_INDEX
@brief softkey序号
*/
typedef uint16  GUISOFTKEY_INDEX;


/*! @typedef GUI_BUTTON_FRAME_TYPE_T
@brief softkey架构模式信息
@note 这个结构保留，目前不支持这项功能
*/
typedef enum
{
    BUTTON_FRAME_RECT,  
    BUTTON_FRAME_3D,   
    BUTTON_FRAME_IMAGE, 
    BUTTON_FRAME_NULL   
}GUI_BUTTON_FRAME_TYPE_T; 

/*!  @typedef GUISK_DATA_TYPE_E
@brief softkey显示模式
@note 目前只显示下列中的一种，不支持两种同时显示
*/
typedef enum
{
    GUISK_DATA_TEXT_ID,     /*!< 显示文本 */ 
	GUISK_DATA_TEXT_BUFFER, /*!< 显示文本 */  
    GUISK_DATA_IMAGE_ID,    /*!< 显示icon */  

    GUISK_DATA_MAX,
}GUISK_DATA_TYPE_E;

/*! @union GUISK_CONTENT_DATA_U
@brief softkey显示的信息
@note 只显示其中一种数据
*/
typedef union
{
    MMI_TEXT_ID_T               text_id;    /*!< text_id */
    MMI_STRING_T                text_buffer;/*!< text_buffer */
    MMI_IMAGE_ID_T              image_id;   /*!< image_id */
}GUISK_CONTENT_DATA_U;

/*! @struct GUISK_CONTENT_T
@brief  控件的内容数据信息
*/
typedef struct
{
    GUISK_DATA_TYPE_E data_type;    /*!< 文本id或者buffer */ 
    GUISK_CONTENT_DATA_U   data_u;  /*!< 文本信息 */ 
} GUISK_CONTENT_T;

/*! @struct GUI_BUTTON_T
@brief  softkey 的 button数据
*/
typedef struct
{
    GUISK_CONTENT_T             content;        /*!< 控件文本信息 */ 
    MMI_IMAGE_ID_T              icon_down_id;   /*!< 按下图片id */ 
    MMI_IMAGE_ID_T              icon_up_id;     /*!< 恢复图片id */ 
	MMI_IMAGE_ID_T              gray_icon_id;   /*!< 灰化图片id */  
    BOOLEAN                     is_pressed;     /*!< 是否被选中 */ 
    BOOLEAN	                    is_show;        /*!< 是否显示 */      
    BOOLEAN                     is_gray;        /*!< 是否灰化 */       
	BOOLEAN	                    is_long_press;  /*!< 是否长按 */ 		 
	GUI_RECT_T                  rect;           /*!< 区域 */ 
}GUI_BUTTON_T;

typedef enum
{
    GUISOFTKEY_BUTTONSTYLE_NONE,

    //pda的3种button风格
	GUISOFTKEY_BUTTONSTYLE_1BUTTON,
    GUISOFTKEY_BUTTONSTYLE_2BUTTON,
    GUISOFTKEY_BUTTONSTYLE_3BUTTON,

	GUISOFTKEY_BUTTONSTYLE_MAX
}GUISOFTKEY_BUTTONSTYLE_E;

typedef enum
{
    // 普通风格
    GUISOFTKEY_STYLE_BAR,

    // button风格
    GUISOFTKEY_STYLE_BUTTON,

	GUISOFTKEY_STYLE_MAX
}GUISOFTKEY_STYLE_E;

/*! @struct GUISOFTKEY_IMG_T
@brief  softkey背景信息
*/
typedef struct 
{
	MMI_IMAGE_ID_T      bg_img;         /*!< 背景图片 */
    MMI_IMAGE_ID_T      left_up_img;    /*!< 左软键up图片 */
    MMI_IMAGE_ID_T      left_down_img;  /*!< 左软键按下图片 */
    MMI_IMAGE_ID_T      mid_up_img;     /*!< 中间软键up图片 */
    MMI_IMAGE_ID_T      mid_down_img;   /*!< 中间键按下图片 */
    MMI_IMAGE_ID_T      right_up_img;   /*!< 右键up图片 */
    MMI_IMAGE_ID_T      right_down_img; /*!< 右软键按下图片 */
} GUISOFTKEY_IMG_T;

typedef struct
{
    MMI_TEXT_ID_T		leftsoft_id;    /*!< 左软键 */
	MMI_TEXT_ID_T		midsoft_id;     /*!< 中间软键 */
	MMI_TEXT_ID_T		rightsoft_id;   /*!< 右软键 */  
} GUISOFTKEY_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
