/*****************************************************************************
** File Name:      ctrldateedit_export.h                                        *
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

#ifndef _CTRLDATEEDIT_EXPORT_H_
#define _CTRLDATEEDIT_EXPORT_H_ 

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

/*! @enum GUIEDIT_DATE_STYLE_E
@brief ���ڱ༭������
*/
typedef enum
{
    GUIEDIT_DATE_STYLE_NULL,    /*!<����λ*/
    GUIEDIT_DATE_STYLE_YMD,     /*!<��-��-��*/
    GUIEDIT_DATE_STYLE_YDM,     /*!<��-��-��*/
    GUIEDIT_DATE_STYLE_MYD,     /*!<��-��-��*/
    GUIEDIT_DATE_STYLE_MDY,     /*!<��-��-��*/
    GUIEDIT_DATE_STYLE_DYM,     /*!<��-��-��*/
    GUIEDIT_DATE_STYLE_DMY,     /*!<��-��-��*/
    GUIEDIT_DATE_STYLE_MAX      /*!<����λ*/
} GUIEDIT_DATE_STYLE_E;

/*! @enum GUIEDIT_DATE_YEAR_RANGE_E
@brief ���ڷ�Χ
*/
typedef enum
{
    GUIEDIT_DATE_YEAR_RANGE_NULL,   /*!<����λ*/
    GUIEDIT_DATE_YEAR_RANGE_SET,    /*!<����*/
    GUIEDIT_DATE_YEAR_RANGE_QUERY,  /*!<��ѯ*/
    GUIEDIT_DATE_YEAR_RANGE_MAX     /*!<����λ*/
} GUIEDIT_DATE_YEAR_RANGE_E;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get date
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDATEEDIT_GetDate (
    MMI_CTRL_ID_T   ctrl_id,    //in
    uint16          *year_ptr,  //in/out:may PNULL
    uint8           *month_ptr, //in/out:may PNULL
    uint8           *day_ptr    //in/out:may PNULL
);

/*****************************************************************************/
//  Description : set date
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDATEEDIT_SetDate (
    MMI_CTRL_ID_T   ctrl_id,
    uint16          year,
    uint8           month,
    uint8           day
);

/*****************************************************************************/
//  Description : set date style and separator
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDATEEDIT_SetDateStyle (
    MMI_CTRL_ID_T          ctrl_id,
    GUIEDIT_SEPARATOR_E    *separator_ptr, //may PNULL
    GUIEDIT_DATE_STYLE_E   *date_style_ptr,//may PNULL
    BOOLEAN                is_update
);

/*****************************************************************************/
//  Description : get date range
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIEDIT_DATE_YEAR_RANGE_E CTRLDATEEDIT_GetDateRange (
    MMI_CTRL_ID_T     ctrl_id
);

/*****************************************************************************/
//  Description : set date range
//  Global resource dependence :
//  Author: Jassmine
//  Note: default is GUIEDIT_DATE_YEAR_RANGE_SET
/*****************************************************************************/
PUBLIC void CTRLDATEEDIT_SetDateRange (
    MMI_CTRL_ID_T              ctrl_id,
    GUIEDIT_DATE_YEAR_RANGE_E  year_range_type
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
