/*****************************************************************************
** File Name:       ctrlwfday.h                                              *
** Author:          qi.zhang                                                 *
** Date:            08/30/2021                                               *
** Copyright:                                                                *
** Description:    This file is used to define watch face control            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/30/2021         qi.zhang                Create                         *
******************************************************************************/

#ifndef _CTRLWFDAY_H_
#define _CTRLWFDAY_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlwfcommon.h"


/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  WATCHFACE_DAY_LIST_NUM            (10) //number[0~9]

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
    DAY_ITEM_NUM0,
    DAY_ITEM_NUM1,
    DAY_ITEM_MAX
}WATCHFACE_DAY_ITEM_E;


typedef struct
{
    MMI_CTRL_ID_T     ctrl_id;
    GUI_POINT_T       center_point;
    GUI_RECT_T              rect;
    uint8             value;
}WATCHFACE_DAY_INFO_T;


typedef struct
{
    WATCHFACE_COMMON_BASE_ATTR

    wchar                    day_number[WATCHFACE_DAY_LIST_NUM][WATCHFACE_IMAGE_FULL_PATH_MAX_LEN];
    BOOLEAN                  double_digital;
    uint8                    timer_id;
    WATCHFACE_DAY_INFO_T     self_info[DAY_ITEM_MAX];
}WATCHFACE_DAY_OBJ_T;

#endif



