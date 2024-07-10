/*****************************************************************************
** File Name:       ctrlwfdigitaltime.h                                      *
** Author:          yuming.yang                                              *
** Date:            08/13/2021                                               *
** Copyright:                                                                *
** Description:    This file is used to define watch face control            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/13/2021       yuming.yang               Create                         *
******************************************************************************/

#ifndef _CTRLWFDIGITALTIME_H_
#define _CTRLWFDIGITALTIME_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlwfcommon.h"


/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  WATCHFACE_DIGITAL_LIST_NUM            (10) //number[0~9]


/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
    DIGITAL_TIME_HOUR_HIGH,
    DIGITAL_TIME_HOUR_LOW,
    DIGITAL_TIME_COLON,
    DIGITAL_TIME_MINUTE_HIGH,
    DIGITAL_TIME_MINUTE_LOW,
    DIGITAL_TIME_ITEM_MAX
}WATCHFACE_DIGITAL_TIME_ITEM_E;


typedef struct
{
    MMI_CTRL_ID_T     ctrl_id;
    GUI_POINT_T       center_point;
    GUI_RECT_T          rect;
    uint8             time_value;
}WATCHFACE_DIGITAL_TIME_INFO_T;


typedef struct
{
    WATCHFACE_COMMON_BASE_ATTR

    WATCHFACE_ALL_TYPE_E            type;
    uint32                          time_out;
    wchar                           img_number[WATCHFACE_DIGITAL_LIST_NUM][WATCHFACE_IMAGE_FULL_PATH_MAX_LEN];
    wchar                           img_colon[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN];
    uint8                           timer_id;
    GUI_RECT_T          rect;
    WATCHFACE_DIGITAL_TIME_INFO_T   self_info[DIGITAL_TIME_ITEM_MAX];
}WATCHFACE_DIGITAL_TIME_OBJ_T;



/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                */
/*---------------------------------------------------------------------------*/





#endif


