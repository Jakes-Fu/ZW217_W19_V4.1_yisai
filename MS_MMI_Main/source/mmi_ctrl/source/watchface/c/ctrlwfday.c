/*****************************************************************************
** File Name:       ctrlwf_day.c                                             *
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
#include "ctrlwfday.h"
#include "ctrlwf_export.h"
#include "ctrlwfcommon.h"
#include "mmk_timer.h"
#include "dal_time.h"



/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

#define DAY_TIMER_OUT          (60000) //day的刷新时间

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : creat day data
//  Parameter: [In] win_id
//             [In] double_digital:当日期只有个位时，十位数是否显示
//             [In] p_image_list: 图片路径
//             [Out] None
//             [Return] WATCHFACE_DAY_OBJ_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_DAY_OBJ_T* CreateDayData(
                                        MMI_WIN_ID_T              win_id,
                                        BOOLEAN                   double_digital,
                                        char*                     p_image_list[],
                                        float                     scale
                                        );

/*****************************************************************************/
//  Description : display day
//  Parameter: [In] win_id
//             [In] p_day_info:有关天的所有信息
//             [In] is_create:ture第一次创建
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayDay(
                MMI_WIN_ID_T                 win_id,
                WATCHFACE_DAY_OBJ_T*         p_day_info,
                BOOLEAN                      is_create
                );
/*****************************************************************************/
//  Description : update day
//  Parameter: [In] timer_id
//             [In] param
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void UpdateDay(uint8 timer_id, uint32 param);
/*****************************************************************************/
//  Description : creat day data
//  Parameter: [In] win_id
//             [In] double_digital:当日期只有个位时，十位是否显示
//             [In] p_image_list: 图片路径
//             [Out] None
//             [Return] WATCHFACE_DAY_OBJ_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_DAY_OBJ_T* CreateDayData(
                                        MMI_WIN_ID_T              win_id,
                                        BOOLEAN                   double_digital,
                                        char*                     p_image_list[],
                                        float                     scale
                                        )
{
    WATCHFACE_DAY_OBJ_T*  p_day_info = PNULL;
    wchar                 day_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN] = {0};
    uint16                wstr_len = 0;
    uint8                 i = 0;

    if(PNULL == p_image_list)
    {
        CTRL_WF_TRACE("p_image_list = pnull!");
        return PNULL;
    }
    //alloc space
    p_day_info = SCI_ALLOC_APPZ(sizeof(WATCHFACE_DAY_OBJ_T));
    if (PNULL == p_day_info)
    {
        CTRL_WF_TRACE("The p_day_info is PNULL!");
        return PNULL;
    }
    SCI_MEMSET(p_day_info, 0, sizeof(WATCHFACE_DAY_OBJ_T));

    //store base info
    p_day_info->win_id = win_id;
    p_day_info->double_digital = double_digital;
    p_day_info->scale = scale;

    for(i = 0; i < WATCHFACE_DAY_LIST_NUM; i++)
    {
        if(PNULL != p_image_list[i])
        {
            MMIAPICOM_StrToWstr((uint8 *)p_image_list[i], day_wstr);
            wstr_len = MMIAPICOM_Wstrlen(day_wstr);
            MMI_WSTRNCPY(p_day_info->day_number[i], WATCHFACE_IMAGE_FULL_PATH_MAX_LEN,day_wstr, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, wstr_len);
        }
        else
        {
            CTRL_WF_TRACE("p_image_list[%d] is PNULL!", i);
        }
    }
    return p_day_info;
}
/*****************************************************************************/
//  Description : display day
//  Parameter: [In] win_id
//             [In] p_day_info:有关天的所有信息
//             [In] center_position: 中心坐标
//             [In] is_create
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayDay(
                MMI_WIN_ID_T                 win_id,
                WATCHFACE_DAY_OBJ_T*         p_day_info,
                BOOLEAN                      is_create
                )
{
    SCI_DATE_T              sys_time = {0};
    wchar*                  p_img_number[WATCHFACE_DAY_LIST_NUM] = {PNULL};
    GUI_POINT_T             center_point = {0};
    GUI_RECT_T              disp_rect = {0};
    uint8                   item_value[DAY_ITEM_MAX] = {0};
    uint8                   i = 0;

    if (PNULL == p_day_info)
    {
        CTRL_WF_TRACE("p_day_info = pnull!");
        return;
    }
    TM_GetSysDate(&sys_time);
    item_value[DAY_ITEM_NUM0] = sys_time.mday / 10;
    item_value[DAY_ITEM_NUM1] = sys_time.mday % 10;

    for(i = 0; i < WATCHFACE_DAY_LIST_NUM; i++)
    {
        p_img_number[i] = p_day_info->day_number[i];
    }

    CTRL_WF_TRACE("day_num0 = %d,day_num1 = %d,double_digital = %d",
                  p_day_info->self_info[DAY_ITEM_NUM0].value,
                  p_day_info->self_info[DAY_ITEM_NUM1].value,
                  p_day_info->double_digital);
    if(TRUE == is_create)
    {
        p_day_info->self_info[DAY_ITEM_NUM0].value = item_value[DAY_ITEM_NUM0];
        p_day_info->self_info[DAY_ITEM_NUM1].value = item_value[DAY_ITEM_NUM1];
        if(FALSE == p_day_info->double_digital && 0 == p_day_info->self_info[DAY_ITEM_NUM0].value)
        {
            center_point.x = p_day_info->self_info[DAY_ITEM_NUM0].center_point.x/2 + p_day_info->self_info[DAY_ITEM_NUM1].center_point.x/2;
            center_point.y = p_day_info->self_info[DAY_ITEM_NUM0].center_point.y;
            disp_rect = p_day_info->self_info[DAY_ITEM_NUM0].rect;
            disp_rect.left = disp_rect.left + center_point.x - p_day_info->self_info[DAY_ITEM_NUM0].center_point.x;
            disp_rect.right = disp_rect.right + center_point.x - p_day_info->self_info[DAY_ITEM_NUM0].center_point.x;
            CTRLWF_Common_CreateImg(win_id,
                                    p_day_info->self_info[DAY_ITEM_NUM1].ctrl_id,
                                    &disp_rect,
                                    p_img_number[p_day_info->self_info[DAY_ITEM_NUM1].value],
                                    p_day_info->scale,
                                    &(p_day_info->layer)
                                    );
        }
        else
        {
            //CreateImg
            CTRLWF_Common_CreateImg(win_id,
                                    p_day_info->self_info[DAY_ITEM_NUM0].ctrl_id,
                                    &p_day_info->self_info[DAY_ITEM_NUM0].rect,
                                    p_img_number[p_day_info->self_info[DAY_ITEM_NUM0].value],
                                    p_day_info->scale,
                                    &(p_day_info->layer)
                                    );
            CTRLWF_Common_CreateImg(win_id,
                                    p_day_info->self_info[DAY_ITEM_NUM1].ctrl_id,
                                    &p_day_info->self_info[DAY_ITEM_NUM1].rect,
                                    p_img_number[p_day_info->self_info[DAY_ITEM_NUM1].value],
                                    p_day_info->scale,
                                    &(p_day_info->layer)
                                    );
        }
    }
    else
    {
        if(item_value[DAY_ITEM_NUM0] != p_day_info->self_info[DAY_ITEM_NUM0].value || item_value[DAY_ITEM_NUM1] != p_day_info->self_info[DAY_ITEM_NUM1].value)
        {
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_day_info->layer));
#endif
            p_day_info->self_info[DAY_ITEM_NUM0].value = item_value[DAY_ITEM_NUM0];
            p_day_info->self_info[DAY_ITEM_NUM1].value = item_value[DAY_ITEM_NUM1];
            if(FALSE == p_day_info->double_digital && 0 == p_day_info->self_info[DAY_ITEM_NUM0].value)
            {
                center_point.x = p_day_info->self_info[DAY_ITEM_NUM0].center_point.x/2 + p_day_info->self_info[DAY_ITEM_NUM1].center_point.x/2;
                center_point.y = p_day_info->self_info[DAY_ITEM_NUM0].center_point.y;
                disp_rect = p_day_info->self_info[DAY_ITEM_NUM0].rect;
                disp_rect.left = disp_rect.left + center_point.x - p_day_info->self_info[DAY_ITEM_NUM0].center_point.x;
                disp_rect.right = disp_rect.right + center_point.x - p_day_info->self_info[DAY_ITEM_NUM0].center_point.x;
                CTRLWF_Common_UpdateImg(p_day_info->self_info[DAY_ITEM_NUM1].ctrl_id, &disp_rect, p_img_number[p_day_info->self_info[DAY_ITEM_NUM1].value], p_day_info->scale, &(p_day_info->layer));
            }
            else
            {
                //CreateImg
                CTRLWF_Common_UpdateImg(p_day_info->self_info[DAY_ITEM_NUM0].ctrl_id, &p_day_info->self_info[DAY_ITEM_NUM0].rect, p_img_number[p_day_info->self_info[DAY_ITEM_NUM0].value], p_day_info->scale, &(p_day_info->layer));
                CTRLWF_Common_UpdateImg(p_day_info->self_info[DAY_ITEM_NUM1].ctrl_id, &p_day_info->self_info[DAY_ITEM_NUM1].rect, p_img_number[p_day_info->self_info[DAY_ITEM_NUM1].value], p_day_info->scale, &(p_day_info->layer));
            }
        }
    }

}
/*****************************************************************************/
//  Description : update day
//  Parameter: [In] timer_id
//             [In] param
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
LOCAL void UpdateDay(uint8 timer_id, uint32 param)
{
    WATCHFACE_DAY_OBJ_T*   p_day_info = (WATCHFACE_DAY_OBJ_T*) param;

    if (PNULL == p_day_info)
    {
        CTRL_WF_TRACE("param = pnull!");
        MMK_StopTimer(timer_id);
        timer_id = 0;
        return;
    }

    if (TRUE == MMK_IsFocusWin(p_day_info->win_id))
    {
        if (0 != p_day_info->timer_id)
        {
            if(timer_id == p_day_info->timer_id)
            {
                //update time
                DisplayDay(p_day_info->win_id, p_day_info, FALSE);
#if !defined(UI_MULTILAYER_SUPPORT) ||defined(WF_USE_SINGLELAYER)
                MMK_PostMsg(p_day_info->win_id, MSG_FULL_PAINT, PNULL, 0); //update all ctrl
#endif

            }
        }
    }
}
/*****************************************************************************/
//  Description : create watch face day ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] double_digital:十位为0时，是否显示
//             [In] center_position: 中心坐标
//             [In] p_image_list[]: 资源路径
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T  CTRLWF_Day_Create (
                                                MMI_WIN_ID_T                 win_id,
                                                MMI_CTRL_ID_T                ctrl_id,
                                                BOOLEAN                      double_digital,
                                                GUI_POINT_T                  center_point,
                                                char*                        p_image_list[],
                                                float                        scale
                                                )
{
    MMI_HANDLE_T            handle = 0;
    GUIIMG_INFO_T           day_num_info = {0};
    WATCHFACE_DAY_OBJ_T*    p_day_info = PNULL;

    if(PNULL == p_image_list)
    {
        CTRL_WF_TRACE("p_image_list = pnull!");
        return handle;
    }

    //create data
    p_day_info = CreateDayData(win_id, double_digital, p_image_list, scale);
    if (PNULL == p_day_info)
    {
        CTRL_WF_TRACE("p_day_info = pnull!");
        return handle;
    }
    else
    {
        handle = (MMI_HANDLE_T)p_day_info;
        p_day_info->handle = handle;
    }

    //get image width and height
    CTRLWF_Common_GetImgInfoByPath(p_day_info->day_number[0], &day_num_info);
    //set items ctrl id
    p_day_info->self_info[DAY_ITEM_NUM0].ctrl_id = ctrl_id;
    p_day_info->self_info[DAY_ITEM_NUM1].ctrl_id = ctrl_id + 1;

    p_day_info->self_info[DAY_ITEM_NUM0].center_point.x = center_point.x - day_num_info.image_width/2;
    p_day_info->self_info[DAY_ITEM_NUM0].center_point.y = center_point.y;

    p_day_info->self_info[DAY_ITEM_NUM1].center_point.x = center_point.x + day_num_info.image_width/2;
    p_day_info->self_info[DAY_ITEM_NUM1].center_point.y = center_point.y;

#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER)
    //create layer
    CTRLWF_Common_CreateLayer(&(p_day_info->layer), win_id, (day_num_info.image_width * DAY_ITEM_MAX + WATCHFACE_CREATE_LAYER_MARGIN), (day_num_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN));
    if (p_day_info->layer.block_id == UILAYER_NULL_HANDLE)
    {
        CTRL_WF_TRACE("CreateCacheLayer [error].");
        SCI_MEMSET(p_day_info, 0, sizeof(WATCHFACE_DAY_OBJ_T));
        SCI_FREE(p_day_info);
        p_day_info = PNULL;
        return 0;
    }

    //set position
    UILAYER_SetLayerPosition(&(p_day_info->layer),
                            (center_point.x - (day_num_info.image_width * DAY_ITEM_MAX + WATCHFACE_CREATE_LAYER_MARGIN) / 2),
                            (center_point.y - (day_num_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2)
                            );
#else
    MMK_GetWinLcdDevInfo(win_id, &(p_day_info->layer));
#endif
    p_day_info->self_info[DAY_ITEM_NUM0].rect.left = center_point.x - day_num_info.image_width;
    p_day_info->self_info[DAY_ITEM_NUM0].rect.top = (center_point.y - (day_num_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
    p_day_info->self_info[DAY_ITEM_NUM0].rect.right = center_point.x;
    p_day_info->self_info[DAY_ITEM_NUM0].rect.bottom = (center_point.y + (day_num_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
    p_day_info->self_info[DAY_ITEM_NUM1].rect.left = center_point.x;
    p_day_info->self_info[DAY_ITEM_NUM1].rect.top = (center_point.y - (day_num_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
    p_day_info->self_info[DAY_ITEM_NUM1].rect.right = center_point.x + day_num_info.image_width;
    p_day_info->self_info[DAY_ITEM_NUM1].rect.bottom = (center_point.y + (day_num_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2);

    //display week
    DisplayDay(win_id, p_day_info, TRUE);
    if (0 != p_day_info->timer_id)
    {
        MMK_StopTimer(p_day_info->timer_id);
        p_day_info->timer_id = 0;
    }
    p_day_info->timer_id = MMK_CreateTimerCallback(DAY_TIMER_OUT, UpdateDay, (WATCHFACE_DAY_OBJ_T*)p_day_info, TRUE);


    return handle;
}
/*****************************************************************************/
//  Description : destory watch face day ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Day_Destory (MMI_HANDLE_T handle)
{
    BOOLEAN                result = FALSE;
    WATCHFACE_DAY_OBJ_T*   p_day_info = PNULL;

    CTRL_WF_TRACE("destory enter, handle = %d", handle);
    if (0 != handle)
    {
        p_day_info = (WATCHFACE_DAY_OBJ_T*)handle;
        if (handle == p_day_info->handle)
        {
            //stop timer
            MMK_StopTimer(p_day_info->timer_id);
            p_day_info->timer_id = 0;
            //destory ctrl
            MMK_DestroyControl(p_day_info->self_info[DAY_ITEM_NUM0].ctrl_id);
            MMK_DestroyControl(p_day_info->self_info[DAY_ITEM_NUM1].ctrl_id);
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_day_info->layer));
            //release cache layer
            UILAYER_RELEASELAYER(&(p_day_info->layer));
#endif
            //free space
            SCI_MEMSET(p_day_info, 0, sizeof(WATCHFACE_DAY_OBJ_T));
            SCI_FREE(p_day_info);
            p_day_info = PNULL;
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
PUBLIC BOOLEAN CTRLWF_Day_StopTimer (MMI_HANDLE_T handle)
{
    BOOLEAN     result = FALSE;
    WATCHFACE_DAY_OBJ_T*  p_info = PNULL;

    CTRL_WF_TRACE("stop timer enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_DAY_OBJ_T*)handle;
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
PUBLIC void CTRLWF_Day_RestartTimer (MMI_HANDLE_T handle)
{
    WATCHFACE_DAY_OBJ_T*   p_info = PNULL;

    CTRL_WF_TRACE("update enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_DAY_OBJ_T*)handle;
        if(PNULL == p_info)
        {
            CTRL_WF_TRACE("p_info is pnull");
            return;
        }
        if (handle == p_info->handle)
        {
            if (TRUE == MMK_IsFocusWin(p_info->win_id))
            {
                DisplayDay(p_info->win_id, p_info, FALSE);
                if (0 == p_info->timer_id)
                {
                    p_info->timer_id = MMK_CreateTimerCallback(DAY_TIMER_OUT, UpdateDay, (WATCHFACE_DAY_OBJ_T*)p_info, TRUE);
                }
                else
                {
                    CTRL_WF_TRACE("timer_id != 0");
                }
            }
        }
    }
}

