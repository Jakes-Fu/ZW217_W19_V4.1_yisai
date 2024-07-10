/*****************************************************************************
** File Name:      ctrltimepicker_export.h                                        *
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

#ifndef _CTRLTIMEPICKER_EXPORT_H_
#define _CTRLTIMEPICKER_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbasepicker_export.h"

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
/*! @enum GUITIMEPICKER_INIT_DATA_T
@brief 初始化参数
*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<横竖屏区域*/
} GUITIMEPICKER_INIT_DATA_T;

/*! @enum CTRLTIMEPICKER_STYLE_E
@brief 时间类型
*/
typedef enum
{
    CTRLTIMEPICKER_STYLE_NULL,    /*!<保留位*/
    CTRLTIMEPICKER_STYLE_12,      /*!<12小时制*/
    CTRLTIMEPICKER_STYLE_24,      /*!<24小时制*/
    CTRLTIMEPICKER_STYLE_TIMER,   /*!<定时器*/
    CTRLTIMEPICKER_STYLE_MAX      /*!<保留位*/
} CTRLTIMEPICKER_STYLE_E;

/*! @enum CTRLTIMEPICKER_DISPLAY_STYLE_E
@brief 时间编辑框显示类型
*/
typedef enum
{
    CTRLTIMEPICKER_DISPLAY_STYLE_NULL,  /*!<保留位*/
    CTRLTIMEPICKER_DISPLAY_STYLE_HM,    /*!<显示时:分*/
    CTRLTIMEPICKER_DISPLAY_STYLE_HMS,   /*!<显示时:分:秒*/
    CTRLTIMEPICKER_DISPLAY_STYLE_MAX    /*!<保留位*/
} CTRLTIMEPICKER_DISPLAY_STYLE_E;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set time style and display style
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEPICKER_SetTimeStyle (
    MMI_CTRL_ID_T                     ctrl_id,            // [in]
    CTRLTIMEPICKER_STYLE_E            *time_style_ptr,    // [in] may PNULL
    CTRLTIMEPICKER_DISPLAY_STYLE_E    *display_style_ptr, // [in] may PNULL
    BOOLEAN                           is_update           // [in]
);

/*****************************************************************************/
//  Description : set time
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEPICKER_SetTime (
    MMI_CTRL_ID_T      ctrl_id,            // [in]
    uint8              hour,               // [in]
    uint8              minute,             // [in]
    uint8              second              // [in]
);

/*****************************************************************************/
//  Description : get time
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEPICKER_GetTime (
    MMI_CTRL_ID_T      ctrl_id,            // [in]
    uint8              *hour_ptr,          // [out] may PNULL
    uint8              *minute_ptr,        // [out] may PNULL
    uint8              *second_ptr         // [out] may PNULL
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

/*@}*/
