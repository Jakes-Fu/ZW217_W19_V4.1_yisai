/*****************************************************************************
** File Name:                                                    *
** Author:                                                       *
** Date:                                                           *
** Copyright:      All Rights Reserved.                                      *
** Description:                                                                         *
******************************************************************************/

#ifndef _WATCH_COM_PAN_ITEM_
#define _WATCH_COM_PAN_ITEM_

//#include "graphics_draw.h"
//#include "guistring.h"
#include "watch_launcher_common.h"
//#include "mmicom_trace.h"

typedef enum
{
    WATCH_PAN_CONTROLLER_TYPE_1STR_1IMG,
    WATCH_PAN_CONTROLLER_TYPE_2STR,
}WATCH_PAN_CONTROLLER_TYPE_E;
typedef enum
{
    WATCH_PAN_CONTROLLER_DATA_TYPE_HOUR,
    WATCH_PAN_CONTROLLER_DATA_TYPE_MINUTE,
    WATCH_PAN_CONTROLLER_DATA_TYPE_SECOND,
    WATCH_PAN_CONTROLLER_DATA_TYPE_DAY,
    WATCH_PAN_CONTROLLER_DATA_TYPE_WEEK,
    WATCH_PAN_CONTROLLER_DATA_TYPE_STEP_NUMBER,
    WATCH_PAN_CONTROLLER_DATA_TYPE_STAND_NUMBER,
    WATCH_PAN_CONTROLLER_DATA_TYPE_HEART_RATE,
    WATCH_PAN_CONTROLLER_DATA_TYPE_CHARGE,
    WATCH_PAN_CONTROLLER_DATA_TYPE_CALORIE,
}WATCH_PAN_CONTROLLER_DATA_TYPE_E;
typedef struct
{
    WATCH_PAN_CONTROLLER_DATA_TYPE_E strType;
	GUI_RECT_T rectImg;
	MMI_IMAGE_ID_T imgID;
    GUI_RECT_T strRect;
	MMI_STRING_T str;
}WATCH_PAN_CONTROLLER_DATA_1STR_1IMG_T;
typedef struct
{
    WATCH_PAN_CONTROLLER_DATA_TYPE_E str1Type;
    GUI_RECT_T rect1;
    MMI_STRING_T str1;
    WATCH_PAN_CONTROLLER_DATA_TYPE_E str2Type;
    GUI_RECT_T rect2;
    MMI_STRING_T str2;
}WATCH_PAN_CONTROLLER_DATA_2STR_T;

typedef struct
{
    GUI_RECT_T rect;
    WATCH_PAN_CONTROLLER_TYPE_E controllerType;
	void* controllerData;
}WATCHCOM_PAN_DATA_T;
/*****************************************************************************/
//  Discription: 
//  Global resource dependence:
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_PanController(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info, WATCHCOM_PAN_DATA_T data);



#endif
