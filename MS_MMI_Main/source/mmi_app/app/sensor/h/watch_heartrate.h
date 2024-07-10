/*****************************************************************************
** File Name:   watch_heartrate.h                                            *
** Author:      longwei.qiao                                                 *
** Date:        2021/3/8                                                     *
** Copyright:                                                                *
** Description: This file is used to process msg to main window of heartrate *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                  NAME              DESCRIPTION                     *
** 03/08/2021             longwei.qiao         Create                        *
******************************************************************************/
#ifndef _WATCH_HEARTRECT_H_
#define _WATCH_HEARTRECT_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "ual_sensorhub.h"
/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: regester sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE/FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchHeartRate_RegesterSensor(void);
/*****************************************************************************/
//  Description: active sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_sensorhub_res_e
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchHeartRate_ActiveSensor(void);
/*****************************************************************************/
//  Description: deactive sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_sensorhub_res_e
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchHeartRate_DeactiveSensor(void);
/*****************************************************************************/
//  Description: draw heartrate info
//  Parameter: [In] win_id
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchHeartRate_DrawHeartRateInfo(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description: set measure status
//  Parameter: [In] is_measured
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchHeartRate_SetMeasuredStatus(BOOLEAN is_measuring);
/*****************************************************************************/
//  Description: enter heart rate window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchHeartRate_MainWin_Enter(void);
/*****************************************************************************/
//  Description: get heartrate
//  Parameter: [In] is_measured
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC uint32 AdultWatchHeartRate_GetHeartrate(void);

#endif

