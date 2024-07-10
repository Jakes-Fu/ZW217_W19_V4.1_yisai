/*****************************************************************************
** File Name:      watch_common_btn.h                                              *
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

#ifndef _MMICOM_BTN_H_
#define _MMICOM_BTN_H_

#include "sci_types.h"
//#include "caf.h"
#include "mmk_type.h"

/*****************************************************************************/
//  Discription: set position for One Button
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_Btn_SetPos_1Btn(MMI_CTRL_ID_T btn_id);

/*****************************************************************************/
//  Discription: set position for Two Button
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_Btn_SetPos_2Btn(MMI_CTRL_ID_T left_btn_id, MMI_CTRL_ID_T right_btn_id);

/*****************************************************************************/
//  Discription: set position for Three Button
//  Global resource dependence: 
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCOM_Btn_SetPos_3Btn(MMI_CTRL_ID_T left_btn_id, MMI_CTRL_ID_T middle_btn_id, MMI_CTRL_ID_T right_btn_id);

#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Discription: set position for One Button(AdultWatch)
//  Global resource dependence:
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_Btn_SetPos_1Btn(MMI_CTRL_ID_T btn_id);

/*****************************************************************************/
//  Discription: set position for Two Button(AdultWatch)
//  Global resource dependence:
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_Btn_SetPos_2Btn(MMI_CTRL_ID_T left_btn_id, MMI_CTRL_ID_T right_btn_id);

/*****************************************************************************/
//  Discription: set position for Three Button(AdultWatch)
//  Global resource dependence: 
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_Btn_SetPos_3Btn(MMI_CTRL_ID_T left_btn_id, MMI_CTRL_ID_T middle_btn_id, MMI_CTRL_ID_T right_btn_id);
#endif

#endif
