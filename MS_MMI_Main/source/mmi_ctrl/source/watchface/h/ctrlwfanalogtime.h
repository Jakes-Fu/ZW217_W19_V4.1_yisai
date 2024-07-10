/*****************************************************************************
** File Name:      ctrlwfanalogtime.h                                        *
** Author:                                                                   *
** Date:           08/13/2021                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe watch face export API       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2021      yuming.yang          Create                                  *
******************************************************************************/

#ifndef _CTRLWFANALOGTIME_H_
#define _CTRLWFANALOGTIME_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "ctrlwfcommon.h"


/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    void*   data_ptr;
    uint32  data_type;  //0: RGB565, 1: ARGB888
    uint16  width;
    uint16  height;
    int16   start_x;
    int16   start_y;
}WATCHFACE_ALPHA_IMG_T;

typedef struct
{
    WATCHFACE_COMMON_BASE_ATTR

    uint32              update_interval;
    GUI_POINT_T         center_point;
    wchar               img_hands[3][WATCHFACE_IMAGE_FULL_PATH_MAX_LEN];
    uint8               timer_id;
    GUI_RECT_T          rect;
    MMI_CTRL_ID_T        ctrl_id;
}WATCHFACE_ANALOG_TIME_DATA_T;

/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                */
/*---------------------------------------------------------------------------*/



#endif


