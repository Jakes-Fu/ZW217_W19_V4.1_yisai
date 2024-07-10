/*****************************************************************************
** File Name:       ctrlwfbackground.c                                       *
** Author:          yuming.yang                                              *
** Date:            11/13/2021                                               *
** Copyright:                                                                *
** Description:    This file is used to define watch face image control      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/13/2021       yuming.yang               Create                         *
******************************************************************************/

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlwf_export.h"
#include "mmk_type.h"
#include "ctrlwfbackground.h"
#include "mmi_string.h"
#include "mmk_timer.h"
#include "guilcd.h"
#include "guiblock.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/





/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : display bg
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void DisplayBg(WATCHFACE_BG_INFO_T* p_info, BOOLEAN is_create)
{

    CTRL_WF_TRACE("enter. cur bg index = %d", p_info->cur_img_idx);
    if (PNULL == p_info)
    {
        CTRL_WF_TRACE("p_info = pnull!");
        return;
    }
    
    MMK_GetWinLcdDevInfo(p_info->win_id, &p_info->layer);
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_FillColor(&p_info->layer, 0);
    CTRL_WF_TRACE("uilayer fill color, layer_id:%d", p_info->layer.block_id);
#else
    {
        uint16 w, h;
        GUI_RECT_T rect = {0, 0, 0, 0};
        GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &w, &h);
        rect.right = w-1;
        rect.bottom = h-1;
        GUI_FillRect(&p_info->layer, rect, 0);
        CTRL_WF_TRACE("gui fill rect, layer_id:%d", p_info->layer.block_id);
    }
#endif

    if(TRUE == is_create)
    {
        p_info->cur_img_idx = 0;
        CTRLWF_Common_CreateImg(p_info->win_id, p_info->ctrl_id, &p_info->rect, p_info->img_bg[p_info->cur_img_idx], p_info->scale, &p_info->layer);
    }
    else
    {
        if(p_info->img_num>1)
        {
            p_info->cur_img_idx = (p_info->cur_img_idx+1)%p_info->img_num;
            CTRLWF_Common_UpdateImg(p_info->ctrl_id, &p_info->rect,  p_info->img_bg[p_info->cur_img_idx], p_info->scale, &p_info->layer);
        }
        else
        {
            CTRLWF_Common_DisplayImg(p_info->ctrl_id, &p_info->layer);
        }
    }
}


/*****************************************************************************/
//  Description : update bg by timer
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void UpdateBg(uint8 timer_id, uint32 param)
{
    WATCHFACE_BG_INFO_T* p_info = (WATCHFACE_BG_INFO_T*) param;

    if (PNULL == p_info)
    {
        CTRL_WF_TRACE("p_info = pnull!");
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
                DisplayBg(p_info, FALSE);
#if !defined(UI_MULTILAYER_SUPPORT) ||defined(WF_USE_SINGLELAYER)
                MMK_PostMsg(p_info->win_id, MSG_FULL_PAINT, PNULL, 0); //update all ctrl
#endif
            }
        }
    }
}

/*****************************************************************************/
//  Description : create data
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_BG_INFO_T* CreateBgData(MMI_WIN_ID_T              win_id,
                                            MMI_CTRL_ID_T               ctrl_id,
                                            GUI_POINT_T                 center_point,
                                            uint32                      update_interval,
                                            char*                       p_image_list[],
                                            uint8                       list_num,
                                            float                       scale,
                                            BOOLEAN                     is_loop
                                            )
{
    WATCHFACE_BG_INFO_T*    p_bg_info = PNULL;
    wchar                   num_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN] = {0};
    uint16                  wstr_len = 0;
    uint8                   i = 0;

    CTRL_WF_TRACE("Enter, p_image_list is %s", p_image_list[0]);
    //alloc space
    p_bg_info = SCI_ALLOC_APPZ(sizeof(WATCHFACE_BG_INFO_T));
    if(PNULL == p_bg_info)
    {
        CTRL_WF_TRACE("The p_bg_info is PNULL");
        return PNULL;
    }
    SCI_MEMSET(p_bg_info, 0, sizeof(WATCHFACE_BG_INFO_T));

    //store base info
    p_bg_info->win_id = win_id;
    p_bg_info->ctrl_id = ctrl_id;
    p_bg_info->time_out = update_interval;
    p_bg_info->center_point = center_point;
    p_bg_info->img_num = list_num;
    p_bg_info->scale = scale;
    p_bg_info->is_loop = is_loop;

    for(i = 0; i < list_num; i++)
    {
        if(PNULL != p_image_list[i])
        {
            MMIAPICOM_StrToWstr((uint8 *)p_image_list[i], num_wstr);
            wstr_len = MMIAPICOM_Wstrlen(num_wstr);
            MMI_WSTRNCPY(p_bg_info->img_bg[i], WATCHFACE_IMAGE_FULL_PATH_MAX_LEN,(const wchar*)num_wstr, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, wstr_len);
        }
    }
    return p_bg_info;
}


/*****************************************************************************/
//  Description : create watch face bg ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position: 中心坐标
//             [In] update_interval: 刷新间隔
//             [In] p_image_name: 资源路径
//             [In] list_num: 资源数目
//             [In] scale: 缩放比例
//             [In] is_loop: 是否循环
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLWF_Bg_Create(MMI_WIN_ID_T              win_id,
                                            MMI_CTRL_ID_T               ctrl_id,
                                            GUI_POINT_T                 center_point,
                                            uint32                      update_interval,
                                            char*                       p_image_list[],
                                            uint8                       list_num,
                                            float                       scale,
                                            BOOLEAN                     is_loop
                                            )
{
    MMI_HANDLE_T             handle = 0;
    WATCHFACE_BG_INFO_T*     p_bg_info = PNULL;
    uint16 w=0;
    uint16 h=0;

    CTRL_WF_TRACE("Create Enter, p_image_list is %s", p_image_list[0]);
    if(PNULL == p_image_list)
    {
        CTRL_WF_TRACE("p_image_list = pnull!");
        return handle;
    }

    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &w, &h);
    if(w==0 || h==0)
    {
        CTRL_WF_TRACE("get lcd width height error,[%d][%d]", w, h);
        return handle;
    }

    //create data
    p_bg_info = CreateBgData(win_id, ctrl_id, center_point, update_interval, p_image_list, list_num, scale, is_loop);
    if(PNULL == p_bg_info)
    {
        CTRL_WF_TRACE("p_bg_info is PNULL");
        return handle;
    }
    else
    {
        handle = (MMI_HANDLE_T)p_bg_info;
        p_bg_info->handle = handle;
    }

    p_bg_info->rect.left = 0;
    p_bg_info->rect.right = w-1;
    p_bg_info->rect.top = 0;
    p_bg_info->rect.bottom = h-1;
    //display bg
    DisplayBg(p_bg_info, TRUE);

    if (p_bg_info->img_num > 1)
    {
        if (0 != p_bg_info->timer_id)
        {
            MMK_StopTimer(p_bg_info->timer_id);
            p_bg_info->timer_id = 0;
        }
        p_bg_info->timer_id = MMK_CreateTimerCallback(p_bg_info->time_out, UpdateBg, (WATCHFACE_BG_INFO_T*)p_bg_info, TRUE);
    }

    CTRL_WF_TRACE("Create exit, handle = 0x%x", handle);
    return handle;
}


/*****************************************************************************/
//  Description : destory watch face bg ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Bg_Destory (MMI_HANDLE_T handle)
{
    BOOLEAN     result = FALSE;
    WATCHFACE_BG_INFO_T*  p_info = PNULL;

    CTRL_WF_TRACE("destory enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_BG_INFO_T*)handle;

        if (handle == p_info->handle)
        {
            //stop timer
            if(p_info->timer_id!=0)
            {
                MMK_StopTimer(p_info->timer_id);
                p_info->timer_id = 0;
            }
            //destory ctrl
            MMK_DestroyControl(p_info->ctrl_id);
            //free space
            SCI_MEMSET(p_info, 0, sizeof(WATCHFACE_BG_INFO_T));
            SCI_FREE(p_info);
            p_info = PNULL;
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
PUBLIC BOOLEAN CTRLWF_Bg_StopTimer (MMI_HANDLE_T handle)
{
    BOOLEAN     result = FALSE;
    WATCHFACE_BG_INFO_T*  p_info = PNULL;

    CTRL_WF_TRACE("stop timer enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_BG_INFO_T*)handle;
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
PUBLIC void CTRLWF_Bg_RestartTimer (MMI_HANDLE_T handle)
{
    WATCHFACE_BG_INFO_T*   p_info = PNULL;

    CTRL_WF_TRACE("update enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_BG_INFO_T*)handle;
        if(PNULL == p_info)
        {
            CTRL_WF_TRACE("p_info is pnull");
            return ;
        }
        if (handle == p_info->handle)
        {
            if (TRUE == MMK_IsFocusWin(p_info->win_id))
            {
                if (p_info->img_num > 1)
                {
                    if (0 == p_info->timer_id)
                    {
                        p_info->timer_id = MMK_CreateTimerCallback(p_info->time_out, UpdateBg, (WATCHFACE_BG_INFO_T*)p_info, TRUE);
                    }
                    else
                    {
                        CTRL_WF_TRACE("timer_id != 0");
                    }
                }
                DisplayBg(p_info, FALSE);
            }
        }
    }
}


