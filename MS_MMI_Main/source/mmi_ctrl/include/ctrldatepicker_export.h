/*****************************************************************************
** File Name:      ctrldatepicker_export.h                                        *
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

#ifndef _CTRLDATEPICKER_EXPORT_H_
#define _CTRLDATEPICKER_EXPORT_H_ 

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
/*! @enum GUIDATEPICKER_INIT_DATA_T
@brief 初始化参数
*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<横竖屏区域*/
} GUIDATEPICKER_INIT_DATA_T;


/*! @enum CTRLDATEPICKER_STYLE_E
@brief 日期编辑框类型
*/
typedef enum
{
    CTRLDATEPICKER_STYLE_NULL,    /*!<保留位*/
    CTRLDATEPICKER_STYLE_YMD,     /*!<年-月-日*/
    CTRLDATEPICKER_STYLE_YDM,     /*!<年-日-月*/
    CTRLDATEPICKER_STYLE_MYD,     /*!<月-年-日*/
    CTRLDATEPICKER_STYLE_MDY,     /*!<月-日-年*/
    CTRLDATEPICKER_STYLE_DYM,     /*!<日-年-月*/
    CTRLDATEPICKER_STYLE_DMY,     /*!<日-月-年*/
    CTRLDATEPICKER_STYLE_MAX      /*!<保留位*/
} CTRLDATEPICKER_STYLE_E;

/*! @enum CTRLDATEPICKER_YEAR_RANGE_E
@brief 日期范围
*/
typedef enum
{
    CTRLDATEPICKER_YEAR_RANGE_NULL,   /*!<保留位*/
    CTRLDATEPICKER_YEAR_RANGE_SET,    /*!<设置*/
    CTRLDATEPICKER_YEAR_RANGE_QUERY,  /*!<查询*/
    CTRLDATEPICKER_YEAR_RANGE_MAX     /*!<保留位*/
} CTRLDATEPICKER_YEAR_RANGE_E;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set date
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDATEPICKER_SetDate (
    MMI_CTRL_ID_T   ctrl_id,            // [in]
    uint16          year,               // [in]
    uint8           month,              // [in]
    uint8           day                 // [in]
);

/*****************************************************************************/
//  Description : get date
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDATEPICKER_GetDate (
    MMI_CTRL_ID_T   ctrl_id,        // [in]
    uint16          *year_ptr,      // [out] may PNULL
    uint8           *month_ptr,     // [out] may PNULL
    uint8           *day_ptr        // [out] may PNULL
);

/*****************************************************************************/
//  Description : set date range
//  Global resource dependence :
//  Author: hua.fang
//  Note: default is CTRLDATEPICKER_YEAR_RANGE_SET
/*****************************************************************************/
PUBLIC void CTRLDATEPICKER_SetDateRange (
    MMI_CTRL_ID_T                 ctrl_id,           // [in]
    CTRLDATEPICKER_YEAR_RANGE_E   year_range_type    // [in]
);

/*****************************************************************************/
//  Description : get date range
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC CTRLDATEPICKER_YEAR_RANGE_E CTRLDATEPICKER_GetDateRange (
    MMI_CTRL_ID_T     ctrl_id  // [in]
);

/*****************************************************************************/
//  Description : set date style
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDATEPICKER_SetDateStyle (
    MMI_CTRL_ID_T           ctrl_id,
    CTRLDATEPICKER_STYLE_E  *date_style_ptr,//may PNULL
    BOOLEAN                 is_update
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

/*@}*/
