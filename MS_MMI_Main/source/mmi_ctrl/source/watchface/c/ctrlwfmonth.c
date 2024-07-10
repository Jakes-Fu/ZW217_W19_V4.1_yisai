/*****************************************************************************
** File Name:       ctrlwf_month.c                                           *
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

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "img_dec_interface.h"
#include "mmi_string.h"
#include "ctrlwfcommon.h"
#include "ctrlwfmonth.h"
#include "ctrlwf_export.h"
#include "dal_time.h"
#include "mmk_timer.h"




/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define MONTH_TIMER_OUT          (60000)  //month刷新时间




/**--------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : create month data
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position:中心坐标
//             [In] p_image_list[]:数字资源路径
//             [Out] None
//             [Return] WATCHFACE_MONTH_INFO_T*
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_MONTH_INFO_T* CreateMonthData(MMI_WIN_ID_T      win_id,
                                           MMI_CTRL_ID_T               ctrl_id,
                                           GUI_POINT_T                 center_point,
                                           char*                       p_image_list[],
                                           float                       scale
                                           );

/*****************************************************************************/
//  Description : display month
//  Global resource dependence :
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL void DisplayMonth(WATCHFACE_MONTH_INFO_T*   p_info,
                             BOOLEAN                   is_create
                             );

/*****************************************************************************/
//  Description : update month by timer
//  Global resource dependence : none
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL void UpdateMonth(uint8 timer_id, uint32 param);


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : create month data
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position:中心坐标
//             [In] p_image_list[]:数字资源路径
//             [Out] None
//             [Return] WATCHFACE_MONTH_INFO_T*
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_MONTH_INFO_T* CreateMonthData(MMI_WIN_ID_T      win_id,
                                           MMI_CTRL_ID_T               ctrl_id,
                                           GUI_POINT_T                 center_point,
                                           char*                       p_image_list[],
                                           float                       scale
                                           )
{
    WATCHFACE_MONTH_INFO_T*  p_mon_info = PNULL;
    wchar                    mon_image_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16                   wstr_len = 0;
    uint8                    i = 0;

    if(PNULL == p_image_list)
    {
        CTRL_WF_TRACE("[CreateMonthData]:p_image_list = pnull!");
        return PNULL;
    }

    //alloc space
    p_mon_info = (WATCHFACE_MONTH_INFO_T*)SCI_ALLOC_APPZ(sizeof(WATCHFACE_MONTH_INFO_T));
    if(PNULL == p_mon_info)
    {
        CTRL_WF_TRACE("[CreateMonthData]:The p_mon_info is PNULL");
        return PNULL;
    }
    SCI_MEMSET(p_mon_info, 0, sizeof(WATCHFACE_MONTH_INFO_T));

    //set items info
    p_mon_info->win_id = win_id;
    p_mon_info->ctrl_id = ctrl_id;
    p_mon_info->scale = scale;

    //ratio to point
    p_mon_info->center_point = center_point;

    for(i = 0; i < WATCHFACE_MONTH_LIST_NUM; i++)
    {
        if(PNULL != p_image_list[i])
        {
            MMIAPICOM_StrToWstr((uint8 *)p_image_list[i], mon_image_wstr);
            wstr_len = MMIAPICOM_Wstrlen(mon_image_wstr);
            MMI_WSTRNCPY(p_mon_info->mon_img_number[i], WATCHFACE_IMAGE_FULL_PATH_MAX_LEN,(const wchar*)mon_image_wstr, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, wstr_len);
            SCI_MEMSET(mon_image_wstr, 0, sizeof(mon_image_wstr));
        }
        else
        {
            CTRL_WF_TRACE("[CreateMonthData]p_image_list[i] = pnull! i = %d", i);
        }
    }

    return p_mon_info;
}

/*****************************************************************************/
//  Description : display month
//  Global resource dependence :
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL void DisplayMonth(WATCHFACE_MONTH_INFO_T*   p_info,
                             BOOLEAN                   is_create
                             )
{
    SCI_DATE_T               sys_date = {0};
    uint32                   item_value= 0;

    if (PNULL == p_info)
    {
        CTRL_WF_TRACE("[DisplayMonth]:p_info = pnull!");
        return;
    }

    //get sys date
    TM_GetSysDate(&sys_date);
    item_value = sys_date.mon;

    if(TRUE == is_create)
    {
        p_info->mon_num = item_value;
        CTRLWF_Common_CreateImg(p_info->win_id, p_info->ctrl_id, &p_info->rect, p_info->mon_img_number[p_info->mon_num - 1], p_info->scale, &(p_info->layer));
    }
    else
    {
        if(item_value != p_info->mon_num)
        {
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_info->layer));
#endif
            //update month
            p_info->mon_num = item_value;
            CTRLWF_Common_UpdateImg(p_info->ctrl_id, &p_info->rect, p_info->mon_img_number[p_info->mon_num - 1], p_info->scale, &(p_info->layer));
        }
    }

}

/*****************************************************************************/
//  Description : update month by timer
//  Global resource dependence : none
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL void UpdateMonth(uint8 timer_id, uint32 param)
{
    WATCHFACE_MONTH_INFO_T* p_info = (WATCHFACE_MONTH_INFO_T*) param;

    if (PNULL == p_info)
    {
        CTRL_WF_TRACE("[UpdateMonth]:p_info = pnull!");
        MMK_StopTimer(timer_id);
        timer_id = 0;
        return;
    }

    if (TRUE == MMK_IsFocusWin(p_info->win_id))
    {
        if (0 != p_info->timer_id)
        {
            if(timer_id == p_info->timer_id)
            {
                DisplayMonth(p_info, FALSE);
#if !defined(UI_MULTILAYER_SUPPORT) ||defined(WF_USE_SINGLELAYER)
                MMK_PostMsg(p_info->win_id, MSG_FULL_PAINT, PNULL, 0); //update all ctrl
#endif

            }
        }
    }
}

/*****************************************************************************/
//  Description : create watch face month ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position:中心坐标
//             [In] p_image_list[]:数字资源路径
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: shuting.ma
//  Note::注意需要在外面销毁内存
/*****************************************************************************/
PUBLIC MMI_HANDLE_T  CTRLWF_Month_Create(MMI_WIN_ID_T           win_id,
                                               MMI_CTRL_ID_T              ctrl_id,
                                               GUI_POINT_T                center_point,
                                               char*                      p_image_list[],
                                               float                      scale
                                               )
{
    MMI_HANDLE_T             handle = 0;
    WATCHFACE_MONTH_INFO_T*  p_mon_info = PNULL;
    GUIIMG_INFO_T            img_info = {0};

    if(PNULL == p_image_list)
    {
        CTRL_WF_TRACE("[CTRLWF_Month_Create]:p_image_list = pnull!");
        return handle;
    }

    //create data
    p_mon_info = CreateMonthData(win_id, ctrl_id, center_point, p_image_list, scale);
    if(PNULL == p_mon_info)
    {
        CTRL_WF_TRACE("[CTRLWF_Month_Create]:The p_mon_info is PNULL");
        return handle;
    }
    else
    {
        handle = (MMI_HANDLE_T)p_mon_info;
        p_mon_info->handle = handle;
    }


    CTRLWF_Common_GetImgInfoByPath(p_mon_info->mon_img_number[0], &img_info);

#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER)
    //create layer
    CTRLWF_Common_CreateLayer(&(p_mon_info->layer), win_id, (img_info.image_width + WATCHFACE_CREATE_LAYER_MARGIN), (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN));
    if (p_mon_info->layer.block_id == UILAYER_NULL_HANDLE)
    {
        CTRL_WF_TRACE("CreateCacheLayer [error].");
        SCI_MEMSET(p_mon_info, 0, sizeof(WATCHFACE_MONTH_INFO_T));
        SCI_FREE(p_mon_info);
        p_mon_info = PNULL;
        return 0;
    }
    //set position
    UILAYER_SetLayerPosition(&(p_mon_info->layer),
                            (p_mon_info->center_point.x - (img_info.image_width + WATCHFACE_CREATE_LAYER_MARGIN) / 2),
                            (p_mon_info->center_point.y - (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2)
                            );
#else
    MMK_GetWinLcdDevInfo(win_id, &(p_mon_info->layer));
#endif
    p_mon_info->rect.left = (p_mon_info->center_point.x - (img_info.image_width + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
    p_mon_info->rect.top = (p_mon_info->center_point.y - (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
    p_mon_info->rect.right = (p_mon_info->center_point.x + (img_info.image_width + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
    p_mon_info->rect.bottom = (p_mon_info->center_point.y + (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2);

    //display month
    DisplayMonth(p_mon_info, TRUE);

    if (0 != p_mon_info->timer_id)
    {
        MMK_StopTimer(p_mon_info->timer_id);
        p_mon_info->timer_id = 0;
    }
    p_mon_info->timer_id = MMK_CreateTimerCallback(MONTH_TIMER_OUT, UpdateMonth, (WATCHFACE_MONTH_INFO_T*)p_mon_info, TRUE);

    return handle;
}

/*****************************************************************************/
//  Description : destory watch face month ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Month_Destory (MMI_HANDLE_T handle)
{
    BOOLEAN                  result = FALSE;
    WATCHFACE_MONTH_INFO_T*  p_mon_info = PNULL;

    CTRL_WF_TRACE("[CTRLWF_Month_Destory]:destory enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_mon_info = (WATCHFACE_MONTH_INFO_T*)handle;

        if (handle == p_mon_info->handle)
        {
            //stop timer
            MMK_StopTimer(p_mon_info->timer_id);
            p_mon_info->timer_id = 0;
            //destory ctrl
            MMK_DestroyControl(p_mon_info->ctrl_id);
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_mon_info->layer));
            //release cache layer
            UILAYER_RELEASELAYER(&(p_mon_info->layer));
#endif
            //free space
            SCI_MEMSET(p_mon_info, 0, sizeof(WATCHFACE_MONTH_INFO_T));
            SCI_FREE(p_mon_info);
            p_mon_info = PNULL;
            result = TRUE;
        }
    }

    CTRL_WF_TRACE("[CTRLWF_Month_Destory]:destory exit, result = %d", result);
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
PUBLIC BOOLEAN CTRLWF_Month_StopTimer (MMI_HANDLE_T handle)
{
    BOOLEAN     result = FALSE;
    WATCHFACE_MONTH_INFO_T*  p_info = PNULL;

    CTRL_WF_TRACE("stop timer enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_MONTH_INFO_T*)handle;
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
            else
            {
                CTRL_WF_TRACE("stop timer error, timer_id = 0.");
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
PUBLIC void CTRLWF_Month_RestartTimer (MMI_HANDLE_T handle)
{
    WATCHFACE_MONTH_INFO_T*   p_info = PNULL;

    CTRL_WF_TRACE("update enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_MONTH_INFO_T*)handle;
        if(PNULL == p_info)
        {
            CTRL_WF_TRACE("p_info is pnull");
            return;
        }
        if (handle == p_info->handle)
        {
            if (TRUE == MMK_IsFocusWin(p_info->win_id))
            {
                DisplayMonth(p_info, FALSE);
                if (0 == p_info->timer_id)
                {
                    p_info->timer_id = MMK_CreateTimerCallback(MONTH_TIMER_OUT, UpdateMonth, (WATCHFACE_MONTH_INFO_T*)p_info, TRUE);
                }
                else
                {
                    CTRL_WF_TRACE("timer_id != 0");
                }
            }
        }
    }
}

