/*****************************************************************************
** File Name:       ctrlwfbackground.h                                       *
** Author:          yuming.yang                                              *
** Date:            10/11/2021                                               *
** Copyright:                                                                *
** Description:    This file is used to define watch face control            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 10/11/2021       yuming.yang               Create                         *
******************************************************************************/


#ifndef _CTRLWFBACKGROUND_H_
#define _CTRLWFBACKGROUND_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlwfcommon.h"


/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  WATCHFACE_BG_NUM_MAX            (30)


/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/


typedef struct
{
    WATCHFACE_COMMON_BASE_ATTR

    MMI_CTRL_ID_T   ctrl_id;
    uint32          time_out;
    GUI_POINT_T     center_point;
    wchar           img_bg[WATCHFACE_BG_NUM_MAX][WATCHFACE_IMAGE_FULL_PATH_MAX_LEN];
    uint8           img_num;
    uint8           cur_img_idx;
    uint8           timer_id;
    BOOLEAN         is_loop;
    GUI_RECT_T      rect;
}WATCHFACE_BG_INFO_T;


/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                */
/*---------------------------------------------------------------------------*/





#endif

