/***************************************************************************
** File Name:      mmk_type.h                                              *
** Author:                                                                 *
** Date:           04/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003        Louis.wei        Create								   *
**																		   *
****************************************************************************/
/*!
 *  \addtogroup mmk_type_group_label
 *  @{
 */
 
/*! 
 *  \file 	mmk_type.h
 *  \author	Louis.wei
 *  \date	April,2003
 *  \brief 	This file is used to describe the data struct of 
 *          system, application, window and control
 */

/*!
 * mmk_type.h会被mmk_app.c包含, 如果用户修改需要重新编译mmk_app.c, @liqing.peng, 08-06-26
 */

#ifndef  _MMK_TYPE_H_    
#define  _MMK_TYPE_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "caf.h" 
#include <string.h>


/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define MMI_SOFTICON_NULL		0x00				/*!< soft icon null value */
#define MMI_MIN(_X_, _Y_)		(((_X_) < (_Y_)) ? (_X_) : (_Y_))
													/*!< min macro */
#define MMI_MAX(_X_, _Y_)		(((_X_) > (_Y_)) ? (_X_) : (_Y_))
													/*!< max macro */
#ifdef MMI_ASSERT_MODE
    #ifdef WIN32
        #define MMI_ASSERT_WARNING(_param_)     SCI_ASSERT(0)
    #else
        #define MMI_ASSERT_WARNING SCI_TRACE_HIGH
    #endif
#else
    #define MMI_ASSERT_WARNING      \
        SCI_TRACE_LOW("MMI_ASSERT_WARNING, %s, line=%ld", __FILE__, __LINE__);  \
        SCI_TRACE_LOW
#endif
       
/*! \brief mmi string type */
// typedef struct
// {
//     uint8   *str_ptr;								/*!< string pointer */
//     uint16   length;								/*!< string length in bytes */	
//     BOOLEAN  is_ucs2;								/*!< string is ASCII or UCS2 */
//     
// }MMI_STRING_T;
typedef CAF_STRING_T MMI_STRING_T;

/*! \brief mmi strcpy macro */
#define SCI_STRCPY(_DEST_PTR, _SRC_PTR) \
        SCI_ASSERT(PNULL != (_DEST_PTR) && PNULL != (_SRC_PTR)); \
        strcpy((_DEST_PTR), (_SRC_PTR))


/*! \brief mmi strlen macro */
#define SCI_STRLEN(_STR_PTR) \
        strlen(_STR_PTR)

/*! \brief mmi memcpy macro */
#define MMI_MEMCPY(_DEST_PTR, _DEST_LEN, _SRC_PTR, _SRC_LEN, _SIZE) \
        SCI_PASSERT((_SIZE) <= (_DEST_LEN), ("MMI_MEMCPY: the _DEST_LEN is too small!")); \
        SCI_PASSERT((_SIZE) <= (_SRC_LEN),  ("MMI_MEMCPY: the _SRC_LEN is too small!")); \
        SCI_MEMCPY((_DEST_PTR), (_SRC_PTR), (_SIZE))
/*#define MMI_MEMCPY(_DEST_PTR, _DEST_LEN, _SRC_PTR, _SRC_LEN, _SIZE) \
        SCI_MEMCPY((_DEST_PTR), (_SRC_PTR), \
	((_DEST_LEN) > (_SRC_LEN)) ? MMI_MIN((_SRC_LEN), (_SIZE)) : MMI_MIN((_DEST_LEN), (_SIZE)))*/
        

#define MMI_INVALID_ID   0xFFFFFFFF // include win_id and ctrl_id

//顺序和TP_MSG_E保持一致
//如需要解藕, 需要写个适配函数
typedef enum
{
	MMI_TP_NONE,
	MMI_TP_DOWN,
	MMI_TP_UP,
	MMI_TP_MOVE,
    MMI_TP_GESTURE,
    MMI_TP_SHORT,
	MMI_TP_LONG,
	MMI_TP_PALM,//亮屏状态，覆盖熄屏（TODO）
	MMI_TP_GESTURE_CLICK,//熄屏状态，TP点亮屏幕
	MMI_TP_GESTURE_C,//熄屏状态，画C点亮屏幕（TODO)
	MMI_TP_STATUS_MAX
}MMI_TP_STATUS_E;

typedef enum
{
    MMI_GESTURE_NONE,
    MMI_GESTURE_ZOOM_IN,  //收缩
    MMI_GESTURE_ZOOM_OUT, //伸展
    MMI_GESTURE_ROTATE_LEFT,//向左旋转
    MMI_GESTURE_ROTATE_RIGHT,//向右旋转
    MMI_GESTURE_MAX

}MMI_GESTURE_TYPE_E;

typedef enum
{
    TP_FLING_NONE,
    TP_FLING_UP,
    TP_FLING_DOWN,
    TP_FLING_RIGHT,
    TP_FLING_LEFT,
    TP_FLING_MAX
}MMI_TP_FLING_DIRECTION_E;

#define MMK_MODULE_MAX_NUM 0xFF //use for handle

#ifdef WIN32
	#define _D_FILE_NAME	__FILE__
	#define _D_FILE_LINE	__LINE__
#else
	#if !defined RELEASE_INFO
		#define _D_FILE_NAME	__MODULE__
		#define _D_FILE_LINE	__LINE__
	#else
		#define _D_FILE_NAME	PNULL
		#define _D_FILE_LINE	0
	#endif
#endif

/*----------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                    */
/*----------------------------------------------------------------------------*/
typedef void*					PWND;				/*!< window struct pointer type */
typedef void*					DPARAM;				/*!< param data pointer type */
typedef void*					ADD_DATA;			/*!< additional data pointer type */
typedef uint16					MMI_SOFT_ICON;		/*!< soft icon type */

typedef uint32					MMI_TEXT_ID_T;		/*!< text id type */
typedef uint32					MMI_IMAGE_ID_T;		/*!< image id type */
typedef uint32					MMI_ANIM_ID_T;		/*!< animation id type */
typedef uint32					MMI_RING_ID_T;		/*!< ring id type */
typedef uint32					MMI_COLOR_ID_T;		/*!< color id type */
typedef uint32					MMI_RECT_ID_T;		/*!< rect id type */
typedef uint32					MMI_DATA_ID_T;		/*!< data id type */

typedef uint32					MMI_MENU_GROUP_ID_T;/*!< menu group id type */
typedef uint32					MMI_MENU_ID_T;		/*!< menu id type */
typedef uint16					MMI_MENU_TIP_T;		/*!< menu tip type */
typedef uint32					MMI_MESSAGE_ID_E;	/*!< message id type */
typedef uint32					MMI_WIN_ID_T;		/*!< window id type */
typedef uint32					MMI_CTRL_ID_T;		/*!< control id type */
typedef uint32					MMI_NV_ID_T;		/*!< nv id type */

typedef uint32					MMI_HANDLE_T;		/*!< handle type */

typedef uint32                  MMI_RESULT_E;
#define MMI_RESULT_FALSE        0
#define MMI_RESULT_TRUE         1


typedef enum
{
    GUIANIM_TYPE_NONE,      /*!<?,???*/
    GUIANIM_TYPE_UNKNOWN,   /*!<?????*/
    GUIANIM_TYPE_IMAGE_BMP, /*!<bmp*/
    GUIANIM_TYPE_IMAGE_WBMP,/*!<wbmp*/
    GUIANIM_TYPE_IMAGE_GIF, /*!<gif*/
    GUIANIM_TYPE_IMAGE_JPG, /*!<jpg*/
    GUIANIM_TYPE_IMAGE_PNG, /*!<png*/
    GUIANIM_TYPE_IMAGE_WEBP,/*!<webp*/
    GUIANIM_TYPE_IMAGE_ICO, /*!<ico*/
    GUIANIM_TYPE_ANIM,      /*!<????*/
    GUIANIM_TYPE_ANIM_ARGB, /*!<argb anim*/
    GUIANIM_TYPE_MAX        /*!<???*/
} GUIANIM_TYPE_E;

/*! \brief MMI Window Priority (WIN_ONE_LEVEL is the lowest, WIN_THREE_LEVEL is highest) */
typedef enum
{
    WIN_LOWEST_LEVEL,							/*!< lowest level */
	WIN_ONE_LEVEL,								/*!< one level */
	WIN_TWO_LEVEL,								/*!< middle level */
	WIN_THREE_LEVEL								/*!< highest level */
}MMI_WIN_PRIORITY_E;

/*! \brief the type of the continue move */
typedef enum 
{
    CONTINUE_MOVE_INVALID,
    CONTINUE_MOVE_UP,							/*!< continue move up */
    CONTINUE_MOVE_DOWN,							/*!< continue move down */
    CONTINUE_MOVE_LEFT,							/*!< continue move left */
    CONTINUE_MOVE_RIGHT							/*!< continue move right */    
}CONTINUE_MOVE_TYPE_E;

typedef enum
{
    MMI_SPECIAL_NONE,
    MMI_SPECIAL_RECT,
    MMI_SPECIAL_TEXT,
    MMI_SPECIAL_MAX
}MMI_SPECIAL_BUTTON_TYPE_E;

//control notify parent
// typedef enum
// {
//     MMI_NOTIFY_SET_SOFTKEY,
//     MMI_NOTIFY_RESIZE,
//     MMI_NOTIFY_UPDATE,
//     
//     MMI_NOTIFY_OK,
//     MMI_NOTIFY_PEN_OK,
//     MMI_NOTIFY_CANCEL,
//     MMI_NOTIFY_GET_FOCUS,
//     MMI_NOTIFY_LOST_FOCUS,
// 
//     MMI_NOTIFY_SELECT_CHANGE
// }MMI_NOTIFY_CODE_E;

typedef uint16		        GUI_COLOR_T;
typedef CAF_POINT_T         GUI_POINT_T;
typedef CAF_RECT_T          GUI_RECT_T;
typedef CAF_RECT_T          GUI_PADDING_T;
typedef CAF_BIG_RECT_T      GUI_BIG_RECT_T;
typedef CAF_LCD_DEV_INFO    GUI_LCD_DEV_INFO;	

typedef CAF_BOTH_RECT_T     GUI_BOTH_RECT_T;

//所有的notify消息必须包含此结构
typedef CAF_NOTIFY_T MMI_NOTIFY_T;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
/*! @} */

