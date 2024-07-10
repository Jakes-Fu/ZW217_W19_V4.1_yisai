/*****************************************************************************
** File Name:       ctrlwfmultinum.h                                         *
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

#ifndef _CTRLWFMULTINUM_H_
#define _CTRLWFMULTINUM_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlwfcommon.h"


/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  WATCHFACE_MULTINUM_LIST_NUM            (10) //number[0~9]


/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
    IMG_NUM_ONE,   //显示个位
    IMG_NUM_TWO,   //显示十位
    IMG_NUM_THREE, //显示百位
    IMG_NUM_FOUR,  //显示千位
    IMG_NUM_FIVE,  //显示万位
    IMG_NUM_MAX
}WATCHFACE_MULTINUM_ITEM_E;


typedef struct
{
    WATCHFACE_COMMON_BASE_ATTR

    wchar                img_num[WATCHFACE_MULTINUM_LIST_NUM][WATCHFACE_IMAGE_FULL_PATH_MAX_LEN + 1];
    wchar                img_none[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN + 1];
    uint16               cur_num[IMG_NUM_MAX];
    uint8                timer_id;
    GUI_POINT_T          center_point;
    SensorData_CallBack  getdata_callback;
    MMI_CTRL_ID_T        ctrl_id[IMG_NUM_MAX];
    uint32               timer_interval;
}WATCHFACE_MULTINUM_DATA_T;

/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                */
/*---------------------------------------------------------------------------*/

#endif

