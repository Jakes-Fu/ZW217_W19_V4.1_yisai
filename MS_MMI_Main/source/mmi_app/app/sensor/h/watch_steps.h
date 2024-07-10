/*****************************************************************************
** File Name:   watch_steps.c                                                *
** Author:      longwei.qiao                                                 *
** Date:        2021/3/8                                                     *
** Copyright:                                                                *
** Description: This file is used to process msg to main window of steps     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                  NAME              DESCRIPTION                     *
** 03/08/2021             longwei.qiao         Create                        *
******************************************************************************/
#ifndef _WATCH_STEPS_H_
#define _WATCH_STEPS_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "ual_sensorhub.h"
#include "dal_time.h"


#define STEPS_MAX_LENGTH   5

typedef struct
{
    SCI_DATE_T date;//record cur date
    uint32     steps_count_daily_history;//save daily count ,clear it when the date span greater then one days
    uint32     steps_count_display;//use for display
    uint32     steps_count_sensor_history;//record total steps count ,clear it when power off
}STEPS_DATA_INFO_T;
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
PUBLIC BOOLEAN AdultWatchSteps_RegesterSensor(void);
/*****************************************************************************/
//  Description: active sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_sensorhub_res_e
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchSteps_ActiveSensor(void);
/*****************************************************************************/
//  Description: deactive sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_sensorhub_res_e
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchSteps_DeactiveSensor(void);
/*****************************************************************************/
//  Description: save steps info when power off
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchSteps_SaveInfoWhenPowerOff(void);
/*****************************************************************************/
//  Description: read steps info when power on
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchSteps_ReadInfoWhenPowerOn(void);
/*****************************************************************************/
//  Description: buttin click event
//  Parameter: [In] point
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchSteps_ClickEvent(GUI_POINT_T  point);
/*****************************************************************************/
//  Description: draw steps info
//  Parameter: [In] win_id
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchSteps_DrawStepsInfo(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description: enter auto steps window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchSteps_MainWin_Enter( void );
/*****************************************************************************/
//  Description: get steps count
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC uint32 AdultWatchSteps_GetStepsCounter(void);

#endif

