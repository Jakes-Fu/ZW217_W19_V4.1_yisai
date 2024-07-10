/*****************************************************************************
** File Name:       ctrlwfdigitaldate.h                                      *
** Author:          qi.zhang                                                 *
** Date:            11/01/2021                                               *
** Copyright:                                                                *
** Description:    This file is used to define digital date control          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DIGITALDATE          NAME                  DESCRIPTION                    *
** 11/01/2021           qi.zhang              Create                         *
******************************************************************************/

#ifndef _CTRLWFDIGITALDATE_H_
#define _CTRLWFDIGITALDATE_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlwfcommon.h"


/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  WATCHFACE_DIGITAL_DATE_LIST_NUM            (10) //number[0~9]


/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
    DIGITAL_DATE_YEAR_1,
    DIGITAL_DATE_YEAR_2,
    DIGITAL_DATE_YEAR_3,
    DIGITAL_DATE_YEAR_4,
    DIGITAL_DATE_YEAR_IMAGE,
    DIGITAL_DATE_MONTH_HIGH,
    DIGITAL_DATE_MONTH_LOW,
    DIGITAL_DATE_MONTH_IMAGE,
    DIGITAL_DATE_DAY_HIGH,
    DIGITAL_DATE_DAY_LOW,
    DIGITAL_DATE_ITEM_MAX
}WATCHFACE_DIGITAL_DATE_ITEM_E;


typedef struct
{
    MMI_CTRL_ID_T     ctrl_id;
    GUI_POINT_T       center_point;
    GUI_RECT_T          rect;
    uint8             time_value; //每张图片对应的数字
}WATCHFACE_DIGITAL_DATE_INFO_T;


typedef struct
{
    WATCHFACE_COMMON_BASE_ATTR

    WATCHFACE_ALL_TYPE_E            type;
    wchar                           img_number[WATCHFACE_DIGITAL_DATE_LIST_NUM][WATCHFACE_IMAGE_FULL_PATH_MAX_LEN];
    wchar                           img_connector[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN];
    uint8                           timer_id;
    WATCHFACE_DIGITAL_DATE_INFO_T   self_info[DIGITAL_DATE_ITEM_MAX];
}WATCHFACE_DIGITAL_DATE_OBJ_T;



/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                */
/*---------------------------------------------------------------------------*/





#endif

