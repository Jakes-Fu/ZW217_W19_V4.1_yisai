/*****************************************************************************
** File Name:       ctrlwfmonth.h                                            *
** Author:          shuting.ma                                               *
** Date:            08/31/2021                                               *
** Copyright:                                                                *
** Description:    This file is used to define watch face control            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/31/2021        shuting.ma               Create                         *
******************************************************************************/

#ifndef _CTRLWFMONTH_H_
#define _CTRLWFMONTH_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlwfcommon.h"


/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  WATCHFACE_MONTH_LIST_NUM            (12) //number[0~11]



/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    WATCHFACE_COMMON_BASE_ATTR

    MMI_CTRL_ID_T     ctrl_id;
    GUI_POINT_T       center_point;
    uint8             mon_num;
    wchar             mon_img_number[WATCHFACE_MONTH_LIST_NUM][WATCHFACE_IMAGE_FULL_PATH_MAX_LEN + 1];
    uint8             timer_id;
    GUI_RECT_T          rect;
}WATCHFACE_MONTH_INFO_T;

/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                */
/*---------------------------------------------------------------------------*/

#endif
