/*****************************************************************************
** File Name:       ctrlwfweek.c                                             *
** Author:          qi.zhang                                                 *
** Date:            08/30/2021                                               *
** Copyright:                                                                *
** Description:    This file is used to define watch face control            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/30/2021           qi.zhang              Create                         *
******************************************************************************/

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "img_dec_interface.h"
#include "mmi_string.h"
#include "ctrlwfweek.h"
#include "ctrlwf_export.h"
#include "ctrlwfcommon.h"
#include "mmk_timer.h"
#include "dal_time.h"



/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define WEEK_TIMER_OUT   (60000) //week的刷新时间
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create week data
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] p_image_list[]: 周的资源路径
//             [Out] None
//             [Return] WATCHFACE_WEEK_INFO_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_WEEK_INFO_T* CreateWeekData(
                                        MMI_WIN_ID_T              win_id,
                                        MMI_CTRL_ID_T             ctrl_id,
                                        GUI_POINT_T               center_point,
                                        char*                     p_image_list[],
                                        float                     scale
                                        );


/*****************************************************************************/
//  Description : display week
//  Parameter: [In] win_id
//             [In] p_week_info
//             [In] is_create: 是否第一次显示
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayWeek(
                MMI_WIN_ID_T                 win_id,
                WATCHFACE_WEEK_INFO_T*       p_week_info,
                BOOLEAN                      is_create
                );
/*****************************************************************************/
//  Description : update week
//  Parameter: [In] timer_id
//             [In] param
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void UpdateWeek(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : create week data
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] p_image_list[]: 周的资源路径
//             [Out] None
//             [Return] WATCHFACE_WEEK_INFO_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_WEEK_INFO_T* CreateWeekData(
                                        MMI_WIN_ID_T              win_id,
                                        MMI_CTRL_ID_T             ctrl_id,
                                        GUI_POINT_T               center_point,
                                        char*                     p_image_list[],
                                        float                     scale
                                        )
{
    WATCHFACE_WEEK_INFO_T*  p_week_info = PNULL;
    wchar                   wday_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN] = {0};
    uint16                  wstr_len = 0;
    uint8                   i = 0;

    if(PNULL == p_image_list)
    {
        CTRL_WF_TRACE("p_image_list = pnull!");
        return PNULL;
    }
    //alloc space
    p_week_info = SCI_ALLOC_APPZ(sizeof(WATCHFACE_WEEK_INFO_T));
    if(PNULL == p_week_info)
    {
        CTRL_WF_TRACE("The week_info is PNULL!");
        return PNULL;
    }
    SCI_MEMSET(p_week_info, 0, sizeof(WATCHFACE_WEEK_INFO_T));

    //store info
    p_week_info->win_id = win_id;
    p_week_info->ctrl_id = ctrl_id;
    p_week_info->scale = scale;
    p_week_info->center_point = center_point;

    for(i = 0; i < WATCHFACE_WDAY_LIST_NUM; i++)
    {
        if(PNULL != p_image_list[i])
        {
            MMIAPICOM_StrToWstr((uint8 *)p_image_list[i], wday_wstr);
            wstr_len = MMIAPICOM_Wstrlen(wday_wstr);
            MMI_WSTRNCPY(p_week_info->img_number[i], WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, wday_wstr, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, wstr_len);
        }
        else
        {
            CTRL_WF_TRACE("p_image_list[%d] is PNULL!", i);
        }
    }
    return p_week_info;
}
/*****************************************************************************/
//  Description : display week
//  Parameter: [In] win_id
//             [In] p_week_info
//             [In] is_create: 是否第一次显示
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayWeek(
                MMI_WIN_ID_T                 win_id,
                WATCHFACE_WEEK_INFO_T*       p_week_info,
                BOOLEAN                      is_create
                )
{
    SCI_DATE_T               sys_time= {0};
    wchar*                   p_img_number[WATCHFACE_WDAY_LIST_NUM] = {PNULL};
    uint8                    item_value = 0;
    uint8                    i = 0;

    if (PNULL == p_week_info)
    {
        CTRL_WF_TRACE("p_week_info = pnull!");
        return;
    }
    TM_GetSysDate(&sys_time);
    item_value = sys_time.wday ;
    for(i = 0; i< WATCHFACE_WDAY_LIST_NUM; i++)
    {
        p_img_number[i] = p_week_info->img_number[i];
    }
    if(TRUE == is_create)
    {
        p_week_info->wday_num = item_value;
        CTRLWF_Common_CreateImg(win_id, p_week_info->ctrl_id, &p_week_info->rect, p_img_number[p_week_info->wday_num], p_week_info->scale, &(p_week_info->layer));
    }
    else
    {
        if(item_value != p_week_info->wday_num)
        {
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_week_info->layer));
#endif
            p_week_info->wday_num = item_value;
            CTRLWF_Common_UpdateImg(p_week_info->ctrl_id, &p_week_info->rect, p_img_number[p_week_info->wday_num], p_week_info->scale, &(p_week_info->layer));
        }
    }
}
/*****************************************************************************/
//  Description : update week
//  Parameter: [In] timer_id
//             [In] param
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void UpdateWeek(uint8 timer_id, uint32 param)
{
    WATCHFACE_WEEK_INFO_T* p_week_info = (WATCHFACE_WEEK_INFO_T*) param;

    if (PNULL == p_week_info)
    {
        CTRL_WF_TRACE("param = pnull!");
        MMK_StopTimer(timer_id);
        timer_id = 0;
        return;
    }

    if (TRUE == MMK_IsFocusWin(p_week_info->win_id))
    {
        if (0 != p_week_info->timer_id)
        {
            if(timer_id == p_week_info->timer_id)
            {
                //update time
                DisplayWeek(p_week_info->win_id, p_week_info,  FALSE);
#if !defined(UI_MULTILAYER_SUPPORT) ||defined(WF_USE_SINGLELAYER)
                MMK_PostMsg(p_week_info->win_id, MSG_FULL_PAINT, PNULL, 0); //update all ctrl
#endif

            }
        }
    }
}
/*****************************************************************************/
//  Description : create watch face week ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position: 中心坐标
//             [In] p_image_list[]: 周的资源路径
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T  CTRLWF_Week_Create (
                                                    MMI_WIN_ID_T                 win_id,
                                                    MMI_CTRL_ID_T                ctrl_id,
                                                    GUI_POINT_T                  center_point,
                                                    char*                        p_image_list[],
                                                    float                        scale
                                                    )
{
    MMI_HANDLE_T             handle = 0;
    WATCHFACE_WEEK_INFO_T*   p_week_info = PNULL;
    GUIIMG_INFO_T            img_info = {0};

    if(PNULL == p_image_list)
    {
        CTRL_WF_TRACE("week_image_list = pnull!");
        return handle;
    }

    //create data
    p_week_info = CreateWeekData(win_id, ctrl_id, center_point, p_image_list, scale);
    if (PNULL == p_week_info)
    {
        CTRL_WF_TRACE("p_week_info = pnull!");
        return handle;
    }
    else
    {
        handle = (MMI_HANDLE_T)p_week_info;
        p_week_info->handle = handle;
    }

    CTRLWF_Common_GetImgInfoByPath(p_week_info->img_number[0], &img_info);

#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER)
    //create layer
    CTRLWF_Common_CreateLayer(&(p_week_info->layer), win_id, (img_info.image_width + WATCHFACE_CREATE_LAYER_MARGIN), (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN));
    if (p_week_info->layer.block_id == UILAYER_NULL_HANDLE)
    {
        CTRL_WF_TRACE("CreateCacheLayer [error].");
        SCI_MEMSET(p_week_info, 0, sizeof(WATCHFACE_WEEK_INFO_T));
        SCI_FREE(p_week_info);
        p_week_info = PNULL;
        return 0;
    }
    //set position
    UILAYER_SetLayerPosition(&(p_week_info->layer),
                            (p_week_info->center_point.x - (img_info.image_width + WATCHFACE_CREATE_LAYER_MARGIN) / 2),
                            (p_week_info->center_point.y - (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2)
                            );
#else
    MMK_GetWinLcdDevInfo(win_id, &(p_week_info->layer));
#endif
    p_week_info->rect.left = (p_week_info->center_point.x - (img_info.image_width + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
    p_week_info->rect.right = (p_week_info->center_point.x + (img_info.image_width + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
    p_week_info->rect.top = (p_week_info->center_point.y - (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
    p_week_info->rect.bottom = (p_week_info->center_point.y + (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2);

    //diaplay week
    DisplayWeek(win_id,p_week_info,TRUE);
    //update week
    if (0 != p_week_info->timer_id)
    {
        MMK_StopTimer(p_week_info->timer_id);
        p_week_info->timer_id = 0;
    }
    p_week_info->timer_id = MMK_CreateTimerCallback(WEEK_TIMER_OUT, UpdateWeek, (WATCHFACE_WEEK_INFO_T*)p_week_info, TRUE);

    return handle;
}
/*****************************************************************************/
//  Description : destory watch face week ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Week_Destory (MMI_HANDLE_T handle)
{
    BOOLEAN                  result = FALSE;
    WATCHFACE_WEEK_INFO_T*   p_week_info = PNULL;

    CTRL_WF_TRACE("destory enter, handle = %d", handle);
    if (0 != handle)
    {
        p_week_info = (WATCHFACE_WEEK_INFO_T*)handle;
        if (handle == p_week_info->handle)
        {
            //stop timer
            MMK_StopTimer(p_week_info->timer_id);
            //destory ctrl
            MMK_DestroyControl(p_week_info->ctrl_id);
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_week_info->layer));
            //release cache layer
            UILAYER_RELEASELAYER(&(p_week_info->layer));
#endif
            //free space
            SCI_MEMSET(p_week_info, 0, sizeof(WATCHFACE_WEEK_INFO_T));
            SCI_FREE(p_week_info);
            p_week_info = PNULL;
            result = TRUE;
        }
    }
    CTRL_WF_TRACE("destory exit, result = %d", result);
    return result;
}
/*****************************************************************************/
//  Description : stop timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:用于lose focus时，停止控件内部timer
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Week_StopTimer (MMI_HANDLE_T handle)
{
    BOOLEAN     result = FALSE;
    WATCHFACE_WEEK_INFO_T*  p_info = PNULL;

    CTRL_WF_TRACE("stop timer enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_WEEK_INFO_T*)handle;
        if(PNULL == p_info)
        {
            CTRL_WF_TRACE("p_info is pnull");
            return FALSE;
        }
        if (handle == p_info->handle)
        {
            if(0 != p_info->timer_id)
            {
                MMK_StopTimer(p_info->timer_id);
                p_info->timer_id = 0;
                result = TRUE;
            }
        }
    }
    CTRL_WF_TRACE("stop timer exit, result = %d", result);
    return result;

}
/*****************************************************************************/
//  Description : restart timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_Week_RestartTimer (MMI_HANDLE_T handle)
{
    WATCHFACE_WEEK_INFO_T*   p_info = PNULL;

    CTRL_WF_TRACE("update enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_WEEK_INFO_T*)handle;
        if(PNULL == p_info)
        {
            CTRL_WF_TRACE("p_info is pnull");
            return;
        }
        if (handle == p_info->handle)
        {
            if (TRUE == MMK_IsFocusWin(p_info->win_id))
            {
                DisplayWeek(p_info->win_id, p_info,  FALSE);
                if (0 == p_info->timer_id)
                {
                    p_info->timer_id = MMK_CreateTimerCallback(WEEK_TIMER_OUT, UpdateWeek, (WATCHFACE_WEEK_INFO_T*)p_info, TRUE);
                }
                else
                {
                    CTRL_WF_TRACE("timer_id != 0");
                }
            }
        }
    }
}

