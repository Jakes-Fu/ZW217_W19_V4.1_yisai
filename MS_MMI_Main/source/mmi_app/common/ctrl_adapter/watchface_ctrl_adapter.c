/*****************************************************************************
** File Name:       watchface_ctrl_adapter.c                                 *
** Author:          yuming.yang                                              *
** Date:            08/23/2021                                               *
** Copyright:                                                                *
** Description:    This file is used to manage watch face ctrl id            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/23/2021       yuming.yang               Create                         *
******************************************************************************/

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "watchface_ctrl_adapter.h"
#include "mmicom_trace.h"
#include "chg_drvapi.h"
#include "guilcd.h"
#ifdef UAL_SENSORHUB_SUPPORT
#include "watch_heartrate.h"
#include "watch_steps.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define WATCHFACE_STEPS_UPDATE_TIMER              (10000)  //10s
#define WATCHFACE_HEARTRATE_UPDATE_TIMER          (10000)  //10s
#define WATCHFACE_POWER_UPDATE_TIMER              (10000)  //10s



/**--------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get usable ctrl id from the section
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetCtrlId(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : get steps counter
//  Parameter: [In] None
//             [Out] param:步数
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetStepsCounter(uint32* param);

/*****************************************************************************/
//  Description : get steps counter
//  Parameter: [In] None
//             [Out] param:心率
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetHeartrate(uint32* param);

/*****************************************************************************/
//  Description : get steps counter
//  Parameter: [In] None
//             [Out] param:电池电量
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetChgValue(uint32* param);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : get usable ctrl id from the section
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetCtrlId(MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T   ctrl_id = WATCH_FACE_BEGIN_CTRL_ID;
    uint32          i = 0;

    for(i = 0; i < WATCHFACE_CTRL_ID_NUM_MAX; i++)
    {
        if(0 == MMK_GetCtrlHandleByWin(win_id, ctrl_id))
        {
            TRACE_APP_COMMON("acquired ctrl_id = %d", ctrl_id);
            return ctrl_id;
        }
        else
        {
            ctrl_id ++;
        }
    }

    return 0;
}

/*****************************************************************************/
//  Description : get steps counter
//  Parameter: [In] None
//             [Out] param:步数
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetStepsCounter(uint32* param)
{
    if(PNULL == param)
    {
        TRACE_WATCHFACE("param = pnull!");
        return FALSE;
    }
#ifdef UAL_SENSORHUB_SUPPORT
    *param = AdultWatchSteps_GetStepsCounter();
#endif
    TRACE_WATCHFACE("StepsCounter = %d",*param);
    return TRUE;
}
/*****************************************************************************/
//  Description : get steps counter
//  Parameter: [In] None
//             [Out] param:心率
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetHeartrate(uint32* param)
{
    if(PNULL == param)
    {
        TRACE_WATCHFACE("param = pnull!");
        return FALSE;
    }
#ifdef UAL_SENSORHUB_SUPPORT
    *param = AdultWatchHeartRate_GetHeartrate();
#endif
    TRACE_WATCHFACE("Heartrate = %d",*param);
    return TRUE;
}
/*****************************************************************************/
//  Description : get steps counter
//  Parameter: [In] None
//             [Out] param:电池电量
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetChgValue(uint32* param)
{
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    if(PNULL == param)
    {
        TRACE_WATCHFACE("param = pnull!");
        return FALSE;
    }
    *param = p_chgmng_info->bat_remain_cap;
    TRACE_WATCHFACE("ChgValue = %d",*param);
    return TRUE;
}

/*****************************************************************************/
//  Description : create watch face image
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_Img_Create(MMI_WIN_ID_T                   win_id,
                                                     WATCHFACE_CTRL_PARAM_T*        p_param
                                                       )
{
    MMI_HANDLE_T    handle = 0;
    MMI_CTRL_ID_T   ctrl_id = 0;
    GUI_POINT_T     center_point = {0};

    if(PNULL == p_param)
    {
        TRACE_APP_COMMON("p_param = pnull!");
        return handle;
    }

    center_point = WATCHFACE_ConvertRatioToPoint(p_param->center_position);

    //get ctrl id
    ctrl_id = GetCtrlId(win_id);
    if(0 != ctrl_id)
    {
        handle = CTRLWF_Img_Create(win_id, ctrl_id, center_point, p_param->p_image_list[0], p_param->scale);
    }
    TRACE_APP_COMMON("ctrl_id = %d, handle = 0x%x", ctrl_id, handle);

    return handle;
}

/*****************************************************************************/
//  Description : destory watch face image
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Img_Destory (MMI_HANDLE_T handle)
{
    return CTRLWF_Img_Destory(handle);
}

/*****************************************************************************/
//  Description : create watch face bg
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_Background_Create(
                                                 MMI_WIN_ID_T                    win_id,
                                                 WATCHFACE_CTRL_PARAM_T*         p_param
                                                   )

{
    MMI_HANDLE_T     handle = 0;
    MMI_CTRL_ID_T    ctrl_id = 0;
    GUI_POINT_T      center_point = {0};
    uint8            list_num = 0;
    GUI_LCD_DEV_INFO dev_info = {0};

    if(PNULL == p_param)
    {
        TRACE_APP_COMMON("p_param = pnull!");
        return handle;
    }
    while(PNULL != p_param->p_image_list[list_num])
    {
        list_num++;
    }
    center_point = WATCHFACE_ConvertRatioToPoint(p_param->center_position);

    //get ctrl id
    ctrl_id = GetCtrlId(win_id);
    MMK_GetWinLcdDevInfo(win_id, &dev_info);
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_FillColor(&dev_info, 0);
#else
    {
        uint16 w, h;
        GUI_RECT_T rect = {0, 0, 0, 0};
        GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &w, &h);
        rect.right = w-1;
        rect.bottom = h-1;
        GUI_FillRect(&dev_info, rect, 0);
    }
#endif
    if(0 != ctrl_id)
    {
        handle = CTRLWF_Bg_Create(win_id, ctrl_id, center_point, p_param->update_interval, p_param->p_image_list, list_num, p_param->scale, TRUE);
    }
    TRACE_APP_COMMON("ctrl_id = %d, handle = 0x%x", ctrl_id, handle);

    return handle;
}

/*****************************************************************************/
//  Description : destory watch face bg
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Background_Destory (MMI_HANDLE_T handle)
{
    return CTRLWF_Bg_Destory(handle);
}

/*****************************************************************************/
//  Description : create watch face digital time
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_DigitalTime_Create (MMI_WIN_ID_T                    win_id,
                                                                WATCHFACE_CTRL_PARAM_T*         p_param
                                                                 )
{
    MMI_HANDLE_T    handle = 0;
    MMI_CTRL_ID_T   ctrl_id = 0;
    GUI_POINT_T     center_point = {0};

    if(PNULL == p_param)
    {
        TRACE_APP_COMMON("full_path_ptr = pnull!");
        return handle;
    }
    center_point = WATCHFACE_ConvertRatioToPoint(p_param->center_position);

    //get ctrl id
    ctrl_id = GetCtrlId(win_id);
    if(0 != ctrl_id)
    {
        handle = CTRLWF_DigitalTime_Create(win_id, ctrl_id, center_point, p_param->update_interval, p_param->p_image_list, p_param->p_image_connector, p_param->scale,  p_param->type);
    }
    TRACE_APP_COMMON("ctrl_id = %d, handle = 0x%x", ctrl_id, handle);

    return handle;
}

/*****************************************************************************/
//  Description : destory watch face digital time
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_DigitalTime_Destory (MMI_HANDLE_T handle)
{
    return CTRLWF_DigitalTime_Destory(handle);
}


/*****************************************************************************/
//  Description : create watch face analog time
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_AnalogTime_Create (MMI_WIN_ID_T                    win_id,
                                                                WATCHFACE_CTRL_PARAM_T*         p_param
                                                                )
{
    MMI_HANDLE_T    handle = 0;
    GUI_POINT_T     center_point = {0};
    MMI_CTRL_ID_T   ctrl_id = 0;

    if(PNULL == p_param)
    {
        TRACE_APP_COMMON("p_image_name = pnull!");
        return handle;
    }

    //get ctrl id
    ctrl_id = GetCtrlId(win_id);


    center_point = WATCHFACE_ConvertRatioToPoint(p_param->center_position);

    handle = CTRLWF_AnalogTime_Create(win_id, ctrl_id, center_point, p_param->update_interval, p_param->p_image_list[0], p_param->p_image_list[1], p_param->p_image_list[2], p_param->scale);
    TRACE_APP_COMMON("handle = 0x%x", handle);

    return handle;
}

/*****************************************************************************/
//  Description : destory watch face analog time
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_AnalogTime_Destory (MMI_HANDLE_T handle)
{
    return CTRLWF_AnalogTime_Destory(handle);
}

/*****************************************************************************/
//  Description : create watch face day
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_Day_Create (MMI_WIN_ID_T               win_id,
                                                      WATCHFACE_CTRL_PARAM_T*    p_param
                                                      )
{
    MMI_HANDLE_T    handle = 0;
    MMI_CTRL_ID_T   ctrl_id = 0;
    GUI_POINT_T     center_point = {0};

    if(PNULL == p_param)
    {
        TRACE_APP_COMMON("full_path_ptr = pnull!");
        return handle;
    }

    center_point = WATCHFACE_ConvertRatioToPoint(p_param->center_position);

    //get ctrl id
    ctrl_id = GetCtrlId(win_id);

    if(0 != ctrl_id)
    {
        handle = CTRLWF_Day_Create(win_id, ctrl_id, p_param->double_digital, center_point, p_param->p_image_list, p_param->scale);
    }
    TRACE_APP_COMMON("ctrl_id = %d, handle = 0x%x", ctrl_id, handle);

    return handle;
}

/*****************************************************************************/
//  Description : destory watch face day
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Day_Destory (MMI_HANDLE_T handle)
{
    return CTRLWF_Day_Destory(handle);
}

/*****************************************************************************/
//  Description : create watch face week
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_Week_Create (MMI_WIN_ID_T            win_id,
                                                        WATCHFACE_CTRL_PARAM_T* p_param
                                                        )
{
    MMI_HANDLE_T    handle = 0;
    MMI_CTRL_ID_T   ctrl_id = 0;
    GUI_POINT_T     center_point = {0};

    if(PNULL == p_param)
    {
        TRACE_APP_COMMON("full_path_ptr = pnull!");
        return handle;
    }

    center_point = WATCHFACE_ConvertRatioToPoint(p_param->center_position);

    //get ctrl id
    ctrl_id = GetCtrlId(win_id);
    if(0 != ctrl_id)
    {
        handle = CTRLWF_Week_Create(win_id, ctrl_id, center_point, p_param->p_image_list, p_param->scale);
    }
    TRACE_APP_COMMON("ctrl_id = %d, handle = 0x%x", ctrl_id, handle);

    return handle;
}

/*****************************************************************************/
//  Description : destory watch face week
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Week_Destory (MMI_HANDLE_T handle)
{
    return CTRLWF_Week_Destory(handle);
}

/*****************************************************************************/
//  Description : create watch face month
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_Month_Create (MMI_WIN_ID_T            win_id,
                                                         WATCHFACE_CTRL_PARAM_T* p_param
                                                         )
{
    MMI_HANDLE_T    handle = 0;
    MMI_CTRL_ID_T   ctrl_id = 0;
    GUI_POINT_T     center_point = {0};

    if(PNULL == p_param)
    {
        TRACE_APP_COMMON("full_path_ptr = pnull!");
        return handle;
    }

    center_point = WATCHFACE_ConvertRatioToPoint(p_param->center_position);

    //get ctrl id
    ctrl_id = GetCtrlId(win_id);
    if(0 != ctrl_id)
    {
        handle = CTRLWF_Month_Create(win_id, ctrl_id, center_point, p_param->p_image_list, p_param->scale);
    }
    TRACE_APP_COMMON("ctrl_id = %d, handle = 0x%x", ctrl_id, handle);

    return handle;
}

/*****************************************************************************/
//  Description : destory watch face week
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Month_Destory (MMI_HANDLE_T handle)
{
    return CTRLWF_Month_Destory(handle);
}

/*****************************************************************************/
//  Description : create watch face multinum
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_MultiNum_Create (MMI_WIN_ID_T              win_id,
                                                             WATCHFACE_CTRL_PARAM_T*   p_param
                                                             )
{
    MMI_HANDLE_T    handle = 0;
    MMI_CTRL_ID_T   ctrl_id = 0;
    GUI_POINT_T     center_point = {0};

    if(PNULL == p_param)
    {
        TRACE_APP_COMMON("full_path_ptr = pnull!");
        return handle;
    }

    center_point = WATCHFACE_ConvertRatioToPoint(p_param->center_position);

    //get ctrl id
    ctrl_id = GetCtrlId(win_id);

    if(0 == ctrl_id)
    {
        TRACE_APP_COMMON("ctrl_id is use up!");
        return handle;
    }

    TRACE_APP_COMMON("data_type = %d",  p_param->type);
    switch (p_param->type)
    {
        case WATCHFACE_TYPE_MULTI_NUM_STEPS:
        {
            handle = CTRLWF_MultiNum_Create(win_id, ctrl_id, WATCHFACE_STEPS_UPDATE_TIMER, (SensorData_CallBack)GetStepsCounter, center_point, p_param->p_image_list, p_param->p_image_none, p_param->scale);
            break;
        }
        case WATCHFACE_TYPE_MULTI_NUM_HEARTRATE:
        {
            handle = CTRLWF_MultiNum_Create(win_id, ctrl_id, WATCHFACE_HEARTRATE_UPDATE_TIMER, (SensorData_CallBack)GetHeartrate, center_point, p_param->p_image_list, p_param->p_image_none, p_param->scale);
            break;
        }
        case WATCHFACE_TYPE_MULTI_NUM_POWER:
        {
            handle = CTRLWF_MultiNum_Create(win_id, ctrl_id, WATCHFACE_POWER_UPDATE_TIMER, (SensorData_CallBack)GetChgValue, center_point, p_param->p_image_list, p_param->p_image_none, p_param->scale);
            break;
        }
        default:
            break;
    }

    TRACE_APP_COMMON("ctrl_id = %d, handle = 0x%x", ctrl_id, handle);

    return handle;
}

/*****************************************************************************/
//  Description : destory watch face week
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_MultiNum_Destory (MMI_HANDLE_T handle)
{
    return CTRLWF_MultiNum_Destory(handle);
}

/*****************************************************************************/
//  Description : create watch face digital date
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_DigitalDate_Create (MMI_WIN_ID_T              win_id,
                                                               WATCHFACE_CTRL_PARAM_T*   p_param
                                                                 )
{
    MMI_HANDLE_T    handle = 0;
    MMI_CTRL_ID_T   ctrl_id = 0;
    GUI_POINT_T     center_point = {0};

    if(PNULL == p_param)
    {
        TRACE_APP_COMMON("p_param = pnull!");
        return handle;
    }

    center_point = WATCHFACE_ConvertRatioToPoint(p_param->center_position);

    //get ctrl id
    ctrl_id = GetCtrlId(win_id);
    if(0 != ctrl_id)
    {
        handle = CTRLWF_DigitalDate_Create(win_id, ctrl_id, center_point, p_param->p_image_list, p_param->p_image_connector, p_param->scale, p_param->type);
    }
    TRACE_APP_COMMON("ctrl_id = %d, handle = 0x%x", ctrl_id, handle);

    return handle;
}

/*****************************************************************************/
//  Description : destory watch face digital date
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_DigitalDate_Destory (MMI_HANDLE_T handle)
{
    TRACE_APP_COMMON("WATCHFACE_DigitalDate_Destory");
    return CTRLWF_DigitalDate_Destory(handle);
}


/*****************************************************************************/
//  Description : convert relative position to absolute position
//  Parameter: [In] center_position
//             [Out] None
//             [Return] GUI_POINT_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T WATCHFACE_ConvertRatioToPoint(WATCHFACE_RATIO_POSITION_T  center_position)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_POINT_T     center_point = {0};

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    //set center point
    center_point.x = (int16)(lcd_width * center_position.ratio_x);
    center_point.y = (int16)(lcd_height * center_position.ratio_y);

    TRACE_APP_COMMON("center_position = [%lf, %lf], center_point = [%d, %d]", center_position.ratio_x, center_position.ratio_y, center_point.x, center_point.y);

    return center_point;
}

