/*****************************************************************************
** File Name:      watch_common_btn.c                                              *
** Author:         yuming.yang                                               *
** Date:           02/12/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2020      yuming.yang              Creat
******************************************************************************/

#define _MMICOM_BTN_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "watch_common_btn.h"
#include "guictrl.h"
#include "guibutton.h"
#include "mmk_type.h"

/**--------------------------------------------------------------------------*
 **                         RECT DEFINITION                                  *
 **--------------------------------------------------------------------------*/

#define  VOID       void

#ifdef SCREEN_SHAPE_CIRCULAR
#define MMICOM_BTN_SINGLE_RECT         DP2PX_RECT(96,240-48-24,96+48,240-24)

#define MMICOM_BTN_TWO_LEFT_RECT       DP2PX_RECT(22,155,22+52,155+52)//DP2PX_RECT(60,240-48-24,60+48,240-24)
#define MMICOM_BTN_TWO_RIGHT_RECT      DP2PX_RECT(168,155,168+52,155+52)//DP2PX_RECT(60+48+24,240-48-24,240-60,240-24)

#define MMICOM_BTN_THREE_LEFT_RECT     DP2PX_RECT(20,171,67,219)
#define MMICOM_BTN_THREE_MIDDLE_RECT   DP2PX_RECT(88,164,151,227)
#define MMICOM_BTN_THREE_RIGHT_RECT    DP2PX_RECT(171,171,219,219)
#else
#define MMICOM_BTN_SINGLE_RECT         DP2PX_RECT(88,164,151,227)

#define MMICOM_BTN_TWO_LEFT_RECT       DP2PX_RECT(32,164,95,227)
#define MMICOM_BTN_TWO_RIGHT_RECT      DP2PX_RECT(144,164,207,227)

#define MMICOM_BTN_THREE_LEFT_RECT     DP2PX_RECT(20,171,67,219)
#define MMICOM_BTN_THREE_MIDDLE_RECT   DP2PX_RECT(88,164,151,227)
#define MMICOM_BTN_THREE_RIGHT_RECT    DP2PX_RECT(171,171,219,219)
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: set position for One Button
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC VOID WatchCOM_Btn_SetPos_1Btn(MMI_CTRL_ID_T btn_id)
{
    GUI_RECT_T  btnRect = MMICOM_BTN_SINGLE_RECT;
    GUIBUTTON_SetRect(btn_id, &btnRect);
}

/*****************************************************************************/
//  Discription: set position for Two Button
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC VOID WatchCOM_Btn_SetPos_2Btn(MMI_CTRL_ID_T left_btn_id, MMI_CTRL_ID_T right_btn_id)
{
    GUI_RECT_T  leftBtnRect = MMICOM_BTN_TWO_LEFT_RECT;
    GUI_RECT_T  rightBtnRect = MMICOM_BTN_TWO_RIGHT_RECT;
    GUIBUTTON_SetRect(left_btn_id, &leftBtnRect);
    GUIBUTTON_SetRect(right_btn_id, &rightBtnRect);
}

/*****************************************************************************/
//  Discription: set position for Three Button
//  Global resource dependence: 
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC VOID WatchCOM_Btn_SetPos_3Btn(MMI_CTRL_ID_T left_btn_id, MMI_CTRL_ID_T middle_btn_id, MMI_CTRL_ID_T right_btn_id)
{
    GUI_RECT_T  leftBtnRect = MMICOM_BTN_THREE_LEFT_RECT;
    GUI_RECT_T  middleBtnRect = MMICOM_BTN_THREE_MIDDLE_RECT;
    GUI_RECT_T  rightBtnRect = MMICOM_BTN_THREE_RIGHT_RECT;
    GUIBUTTON_SetRect(left_btn_id, &leftBtnRect);
    GUIBUTTON_SetRect(middle_btn_id, &middleBtnRect);
    GUIBUTTON_SetRect(right_btn_id, &rightBtnRect);
}
#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Discription: set position for One Button(AdultWatch)
//  Global resource dependence:
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC VOID AdultWatchCOM_Btn_SetPos_1Btn(MMI_CTRL_ID_T btn_id)
{
    GUI_RECT_T  btnRect = MMICOM_BTN_SINGLE_RECT;
    GUIBUTTON_SetRect(btn_id, &btnRect);
}

/*****************************************************************************/
//  Discription: set position for Two Button(AdultWatch)
//  Global resource dependence:
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC VOID AdultWatchCOM_Btn_SetPos_2Btn(MMI_CTRL_ID_T left_btn_id, MMI_CTRL_ID_T right_btn_id)
{
    GUI_RECT_T  leftBtnRect = MMICOM_BTN_TWO_LEFT_RECT;
    GUI_RECT_T  rightBtnRect = MMICOM_BTN_TWO_RIGHT_RECT;
    GUIBUTTON_SetRect(left_btn_id, &leftBtnRect);
    GUIBUTTON_SetRect(right_btn_id, &rightBtnRect);
}

/*****************************************************************************/
//  Discription: set position for Three Button(AdultWatch)
//  Global resource dependence: 
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC VOID AdultWatchCOM_Btn_SetPos_3Btn(MMI_CTRL_ID_T left_btn_id, MMI_CTRL_ID_T middle_btn_id, MMI_CTRL_ID_T right_btn_id)
{
    GUI_RECT_T  leftBtnRect = MMICOM_BTN_THREE_LEFT_RECT;
    GUI_RECT_T  middleBtnRect = MMICOM_BTN_THREE_MIDDLE_RECT;
    GUI_RECT_T  rightBtnRect = MMICOM_BTN_THREE_RIGHT_RECT;
    GUIBUTTON_SetRect(left_btn_id, &leftBtnRect);
    GUIBUTTON_SetRect(middle_btn_id, &middleBtnRect);
    GUIBUTTON_SetRect(right_btn_id, &rightBtnRect);
}
#endif
