/*****************************************************************************
** File Name:      ctrltimeedit_export.h                                        *
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

#ifndef _CTRLTIMEEDIT_EXPORT_H_
#define _CTRLTIMEEDIT_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbasefixed_export.h"

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

/*! @enum GUIEDIT_TIME_STYLE_E
@brief 时间类型
*/
typedef enum
{
    GUIEDIT_TIME_STYLE_NULL,    /*!<保留位*/
    GUIEDIT_TIME_STYLE_12,      /*!<12小时制*/
    GUIEDIT_TIME_STYLE_24,      /*!<24小时制*/
    GUIEDIT_TIME_STYLE_TIMER,   /*!<定时器*/
    GUIEDIT_TIME_STYLE_MAX      /*!<保留位*/
} GUIEDIT_TIME_STYLE_E;

/*! @enum GUIEDIT_TIME_DISPLAY_STYLE_E
@brief 时间编辑框显示类型
*/
typedef enum
{
    GUIEDIT_TIME_DISPLAY_STYLE_NULL,/*!<保留位*/
    GUIEDIT_TIME_DISPLAY_STYLE_HM,  /*!<显示时:分*/
    GUIEDIT_TIME_DISPLAY_STYLE_HMS, /*!<显示时:分:秒*/
    GUIEDIT_TIME_DISPLAY_STYLE_MAX  /*!<保留位*/
} GUIEDIT_TIME_DISPLAY_STYLE_E;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get time
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEEDIT_GetTime (
    MMI_CTRL_ID_T   ctrl_id,    //in
    uint8           *hour_ptr,  //in/out:may PNULL
    uint8           *minute_ptr,//in/out:may PNULL
    uint8           *second_ptr //in/out:may PNULL
);

/*****************************************************************************/
//  Description : set time
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEEDIT_SetTime (
    MMI_CTRL_ID_T   ctrl_id,
    uint8           hour,
    uint8           minute,
    uint8           second
);

/*****************************************************************************/
//  Description : set time style and display style
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEEDIT_SetTimeStyle (
    MMI_CTRL_ID_T                  ctrl_id,
    GUIEDIT_SEPARATOR_E            *separator_ptr,     //may PNULL
    GUIEDIT_TIME_STYLE_E           *time_style_ptr,    //may PNULL
    GUIEDIT_TIME_DISPLAY_STYLE_E   *display_style_ptr, //may PNULL
    BOOLEAN                        is_update
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
