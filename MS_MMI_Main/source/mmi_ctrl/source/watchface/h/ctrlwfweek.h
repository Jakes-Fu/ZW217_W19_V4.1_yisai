/*****************************************************************************
** File Name:       ctrlwfweek.h                                             *
** Author:          qi.zhang                                                 *
** Date:            08/30/2021                                               *
** Copyright:                                                                *
** Description:     This file is used to define watch face control           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/30/2021           qi.zhang              Create                         *
******************************************************************************/

#ifndef _CTRLWFWEEK_H_
#define _CTRLWFWEEK_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlwfcommon.h"


/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  WATCHFACE_WDAY_LIST_NUM            (7) //number[0~6]

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    WATCHFACE_COMMON_BASE_ATTR

    MMI_CTRL_ID_T       ctrl_id;
    GUI_POINT_T         center_point;
    wchar               img_number[WATCHFACE_WDAY_LIST_NUM][WATCHFACE_IMAGE_FULL_PATH_MAX_LEN];
    uint8               timer_id;
    uint8               wday_num;
    GUI_RECT_T          rect;
}WATCHFACE_WEEK_INFO_T;

#endif


