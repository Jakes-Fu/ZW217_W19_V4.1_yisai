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
@brief ��ʼ������
*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<����������*/
} GUIDATEPICKER_INIT_DATA_T;


/*! @enum CTRLDATEPICKER_STYLE_E
@brief ���ڱ༭������
*/
typedef enum
{
    CTRLDATEPICKER_STYLE_NULL,    /*!<����λ*/
    CTRLDATEPICKER_STYLE_YMD,     /*!<��-��-��*/
    CTRLDATEPICKER_STYLE_YDM,     /*!<��-��-��*/
    CTRLDATEPICKER_STYLE_MYD,     /*!<��-��-��*/
    CTRLDATEPICKER_STYLE_MDY,     /*!<��-��-��*/
    CTRLDATEPICKER_STYLE_DYM,     /*!<��-��-��*/
    CTRLDATEPICKER_STYLE_DMY,     /*!<��-��-��*/
    CTRLDATEPICKER_STYLE_MAX      /*!<����λ*/
} CTRLDATEPICKER_STYLE_E;

/*! @enum CTRLDATEPICKER_YEAR_RANGE_E
@brief ���ڷ�Χ
*/
typedef enum
{
    CTRLDATEPICKER_YEAR_RANGE_NULL,   /*!<����λ*/
    CTRLDATEPICKER_YEAR_RANGE_SET,    /*!<����*/
    CTRLDATEPICKER_YEAR_RANGE_QUERY,  /*!<��ѯ*/
    CTRLDATEPICKER_YEAR_RANGE_MAX     /*!<����λ*/
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
