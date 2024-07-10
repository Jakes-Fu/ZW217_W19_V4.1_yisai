/*****************************************************************************
** File Name:       ctrlwf_multinum.c                                        *
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
#include "ctrlwfmultinum.h"
#include "ctrlwf_export.h"
#include "dal_time.h"
#include "mmk_timer.h"
#include "guianim.h"




/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/





/**--------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set ctrl id for items
//  Global resource dependence :
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL void SetMultiNumItemsCtrlId(WATCHFACE_MULTINUM_DATA_T* p_multinum_info, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : create multinum data
//  Parameter: [In] win_id
//             [In] center_position:中心坐标
//             [In] p_image_list[]:数字资源路径
//             [In] image_none:无效数字对应的资源路径
//             [Out] None
//             [Return] WATCHFACE_MULTINUM_OBJ_T*
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_MULTINUM_DATA_T* CreateMultiNumData(MMI_WIN_ID_T   win_id,
                                             uint32                       timer_interval,
                                             GUI_POINT_T                  center_point,
                                             char*                        p_image_list[],
                                             char*                        p_image_none,
                                             float                        scale
                                             );

/*****************************************************************************/
//  Description : update multinum
//  Global resource dependence : none
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL void UpdateMultiNum(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : display multinum
//             [In] p_multinum_info:multinum的数据信息
//             [In] is_create:图片第一次创建的使能开关
//             [Out] None
//             [Return] void
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL void DisplayMultiNum(WATCHFACE_MULTINUM_DATA_T*   p_multinum_info,
                                  BOOLEAN                      is_create
                                  );


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set ctrl id for items
//  Global resource dependence :
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL void SetMultiNumItemsCtrlId(WATCHFACE_MULTINUM_DATA_T* p_multinum_info, MMI_CTRL_ID_T ctrl_id)
{
    uint8                       i = 0;

    if (PNULL == p_multinum_info)
    {
        CTRL_WF_TRACE("[SetMultiNumItemsCtrlId]:p_multinum_info = pnull!");
        return;
    }

    for(i = IMG_NUM_ONE; i < IMG_NUM_MAX; i ++)
    {
        p_multinum_info->ctrl_id[i] = ctrl_id;
        ctrl_id ++;
    }
}

/*****************************************************************************/
//  Description : create multinum data
//  Parameter: [In] win_id
//             [In] center_position:中心坐标
//             [In] p_image_list[]:数字资源路径
//             [In] image_none:无效数字对应的资源路径
//             [Out] None
//             [Return] WATCHFACE_MULTINUM_OBJ_T*
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_MULTINUM_DATA_T* CreateMultiNumData(MMI_WIN_ID_T   win_id,
                                             uint32                       timer_interval,
                                             GUI_POINT_T                  center_point,
                                             char*                        p_image_list[],
                                             char*                        p_image_none,
                                             float                        scale
                                             )
{
    WATCHFACE_MULTINUM_DATA_T*   p_multinum_info = PNULL;
    wchar                        num_image_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar                        none_image_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16                       wstr_len = 0;
    uint8                        i = 0;

    if(PNULL == p_image_list)
    {
        CTRL_WF_TRACE("[CreateMultiNumData]:p_image_list = pnull!");
        return PNULL;
    }

    if(PNULL == p_image_none)
    {
        CTRL_WF_TRACE("[CreateMultiNumData]:p_image_none = pnull!");
        return PNULL;
    }

    //alloc space
    p_multinum_info = (WATCHFACE_MULTINUM_DATA_T*)SCI_ALLOC_APPZ(sizeof(WATCHFACE_MULTINUM_DATA_T));
    if(PNULL == p_multinum_info)
    {
         CTRL_WF_TRACE("The multinum_info is PNULL");
         return PNULL;
    }
    SCI_MEMSET(p_multinum_info, 0, sizeof(WATCHFACE_MULTINUM_DATA_T));

    //store base info
    p_multinum_info->win_id = win_id;
    p_multinum_info->center_point = center_point;
    p_multinum_info->scale = scale;
    p_multinum_info->timer_interval = timer_interval;

    MMIAPICOM_StrToWstr((uint8 *)p_image_none, none_image_wstr);
    wstr_len = MMIAPICOM_Wstrlen(none_image_wstr);
    MMI_WSTRNCPY(p_multinum_info->img_none, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN,(const wchar*)none_image_wstr, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, wstr_len);

    for(i = 0; i < WATCHFACE_MULTINUM_LIST_NUM; i++)
    {
        if(PNULL != p_image_list[i])
        {
            MMIAPICOM_StrToWstr((uint8 *)p_image_list[i], num_image_wstr);
            wstr_len = MMIAPICOM_Wstrlen(num_image_wstr);
            CTRL_WF_TRACE("[CreateMultiNumData]:num_image_wstr = %ls, wstr_len = %d", num_image_wstr, wstr_len);//bug id 1941703 CID 420253
            MMI_WSTRNCPY(p_multinum_info->img_num[i], WATCHFACE_IMAGE_FULL_PATH_MAX_LEN,(const wchar*)num_image_wstr, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, wstr_len);
            SCI_MEMSET(num_image_wstr, 0, sizeof(wchar)*(WATCHFACE_IMAGE_FULL_PATH_MAX_LEN + 1));
        }
        else
        {
            CTRL_WF_TRACE("p_image_list[i] = pnull! i = %d", i);
        }
    }
    return p_multinum_info;
}

/*****************************************************************************/
//  Description : update multinum
//  Global resource dependence : none
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL void UpdateMultiNum(uint8 timer_id, uint32 param)

{
    WATCHFACE_MULTINUM_DATA_T*  p_info = (WATCHFACE_MULTINUM_DATA_T*) param;

    if (PNULL == p_info)
    {
        CTRL_WF_TRACE("[UpdateMultiNum]:param = pnull!");
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
                DisplayMultiNum(p_info, FALSE);
#if !defined(UI_MULTILAYER_SUPPORT) ||defined(WF_USE_SINGLELAYER)
                MMK_PostMsg(p_info->win_id, MSG_FULL_PAINT, PNULL, 0); //update all ctrl
#endif

            }
        }
    }
}

/*****************************************************************************/
//  Description : display multinum
//             [In] p_multinum_info:multinum的数据信息
//             [In] is_create:图片第一次创建的使能开关
//             [Out] None
//             [Return] void
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
LOCAL void DisplayMultiNum(WATCHFACE_MULTINUM_DATA_T*   p_multinum_info,
                                  BOOLEAN                      is_create
                                  )
{
    WATCHFACE_MULTINUM_ITEM_E   cur_digit = IMG_NUM_ONE;  //当前位
    uint16                      cur_num = 0;    //当前位的数据
    WATCHFACE_MULTINUM_ITEM_E   index = IMG_NUM_MAX;
    wchar*                      p_img_num[WATCHFACE_MULTINUM_LIST_NUM] =  {0};
    GUIIMG_INFO_T               num_image_info = {0};
    uint8                       i = 0;
    int16                       max_image_width = 0;
    uint32                      temp_num = 0;   //暂存数据
    GUI_POINT_T                 num_center_point[IMG_NUM_MAX] = {0};
    uint32                      data = 0;
    GUI_RECT_T                  rect = {0};

    if (PNULL == p_multinum_info)
    {
        CTRL_WF_TRACE("[DisplayMultiNum]:p_multinum_info = pnull!");
        return;
    }

    if (PNULL == p_multinum_info->getdata_callback)
    {
        CTRL_WF_TRACE("[DisplayMultiNum]:p_multinum_info->getdata_callback = pnull!");
        return;
    }

    p_multinum_info->getdata_callback(&temp_num);

    if(TRUE == is_create)
    {
        data = temp_num;
        do
        {
            cur_num = (data) % 10;
            data = (data) / 10;
            p_multinum_info->cur_num[cur_digit ++] = cur_num;
        }while(0 != data);

        //get image width and height
        for(i = 0; i < WATCHFACE_MULTINUM_LIST_NUM; i++)
        {
            CTRLWF_Common_GetImgInfoByPath(p_multinum_info->img_num[i], &num_image_info);
            if(max_image_width < num_image_info.image_width)
            {
                max_image_width = num_image_info.image_width * (p_multinum_info->scale);
            }
        }

        //确定个位数字的显示位置
        num_center_point[IMG_NUM_ONE].x= p_multinum_info->center_point.x + (cur_digit - 1) * max_image_width / 2;
        num_center_point[IMG_NUM_ONE].y= p_multinum_info->center_point.y;
        rect.left = num_center_point[IMG_NUM_ONE].x - max_image_width / 2;
        rect.right = num_center_point[IMG_NUM_ONE].x + max_image_width / 2;
        rect.top = num_center_point[IMG_NUM_ONE].y - num_image_info.image_height/2;
        rect.bottom = num_center_point[IMG_NUM_ONE].y + num_image_info.image_height/2;

        for(i = 0; i < WATCHFACE_MULTINUM_LIST_NUM; i++)
        {
            p_img_num[i] = p_multinum_info->img_num[i];
        }
        CTRLWF_Common_CreateImg(p_multinum_info->win_id,
                                p_multinum_info->ctrl_id[IMG_NUM_ONE],
                                &rect,
                                p_img_num[p_multinum_info->cur_num[IMG_NUM_ONE]],
                                p_multinum_info->scale,
                                &(p_multinum_info->layer)
                                );

        //以个位为基准显示其余位的数字
        for(index = IMG_NUM_TWO; index < IMG_NUM_MAX; index++)
        {
            num_center_point[index].x = num_center_point[index - 1].x - max_image_width;
            num_center_point[index].y = p_multinum_info->center_point.y;
            rect.left = num_center_point[index].x - max_image_width / 2;
            rect.right = num_center_point[index].x + max_image_width / 2;
            rect.top = num_center_point[index].y - num_image_info.image_height/2;
            rect.bottom = num_center_point[index].y + num_image_info.image_height/2;

            CTRLWF_Common_CreateImg(p_multinum_info->win_id,
                                    p_multinum_info->ctrl_id[index],
                                    &rect,
                                    p_img_num[p_multinum_info->cur_num[index]],
                                    p_multinum_info->scale,
                                    &(p_multinum_info->layer)
                                    );

#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_multinum_info->layer));
#endif

            if(index >= cur_digit)
            {
                GUIANIM_SetVisible(p_multinum_info->ctrl_id[index], FALSE, TRUE);
            }

        }
    }
    else
    {
        if(temp_num != data)
        {
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_multinum_info->layer));
#endif
            data = temp_num;
            do
            {
                cur_num = (data) % 10;
                data = (data) / 10;
                p_multinum_info->cur_num[cur_digit ++] = cur_num;
            }while(0 != data);

            //get image width and height
            for(i = 0; i < WATCHFACE_MULTINUM_LIST_NUM; i++)
            {
                CTRLWF_Common_GetImgInfoByPath(p_multinum_info->img_num[i], &num_image_info);
                if(max_image_width < num_image_info.image_width)
                {
                    max_image_width = num_image_info.image_width * (p_multinum_info->scale);
                }
            }

            //确定个位数字的显示位置
            num_center_point[IMG_NUM_ONE].x= p_multinum_info->center_point.x + (cur_digit - 1) * max_image_width / 2;
            num_center_point[IMG_NUM_ONE].y= p_multinum_info->center_point.y;
            rect.left = num_center_point[IMG_NUM_ONE].x - max_image_width / 2;
            rect.right = num_center_point[IMG_NUM_ONE].x + max_image_width / 2;
            rect.top = num_center_point[IMG_NUM_ONE].y - num_image_info.image_height/2;
            rect.bottom = num_center_point[IMG_NUM_ONE].y + num_image_info.image_height/2;
            for(i = 0; i < WATCHFACE_MULTINUM_LIST_NUM; i++)
            {
                p_img_num[i] = p_multinum_info->img_num[i];
            }
            CTRLWF_Common_UpdateImg(p_multinum_info->ctrl_id[IMG_NUM_ONE],
                                    &rect,
                                    p_img_num[p_multinum_info->cur_num[IMG_NUM_ONE]],
                                    p_multinum_info->scale, &(p_multinum_info->layer)
                                    );

            //以个位为基准显示其余位的数字
            for(index = IMG_NUM_TWO; index < cur_digit; index++)
            {
                num_center_point[index].x = num_center_point[index - 1].x - max_image_width;
                num_center_point[index].y = p_multinum_info->center_point.y;
                rect.left = num_center_point[index].x - max_image_width / 2;
                rect.right = num_center_point[index].x + max_image_width / 2;
                rect.top = num_center_point[index].y - num_image_info.image_height/2;
                rect.bottom = num_center_point[index].y + num_image_info.image_height/2;

                CTRLWF_Common_UpdateImg(p_multinum_info->ctrl_id[index],
                                        &rect,
                                        p_img_num[p_multinum_info->cur_num[index]],
                                        p_multinum_info->scale, &(p_multinum_info->layer)
                                        );
                GUIANIM_SetVisible(p_multinum_info->ctrl_id[index], TRUE, TRUE);
            }
        }
    }
}

/*****************************************************************************/
//  Description : create watch face multinum ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] timer_interval:刷新间隔
//             [In] getdata_callback:外部传入数据
//             [In] center_position:中心坐标
//             [In] p_image_list[]:数字资源路径
//             [In] p_image_none:无效数字对应的资源路径
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: shuting.ma
//  Note:注意需要在外面销毁内存
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLWF_MultiNum_Create(MMI_WIN_ID_T              win_id,
                                                     MMI_CTRL_ID_T               ctrl_id,
                                                     uint32                      timer_interval,
                                                     SensorData_CallBack         getdata_callback,
                                                     GUI_POINT_T                 center_point,
                                                     char*                       p_image_list[],
                                                     char*                       p_image_none,
                                                     float                       scale
                                                     )
{
    MMI_HANDLE_T                 handle = 0;
    WATCHFACE_MULTINUM_DATA_T*   p_multinum_info = PNULL;
    GUIIMG_INFO_T                img_info = {0};

    if(PNULL == p_image_list)
    {
        CTRL_WF_TRACE("[CTRLWF_MultiNum_Create]:p_image_list = pnull!");
        return handle;
    }

    if(PNULL == p_image_none)
    {
        CTRL_WF_TRACE("[CTRLWF_MultiNum_Create]:p_image_none = pnull!");
        return handle;
    }

    //create data
    p_multinum_info = CreateMultiNumData(win_id, timer_interval, center_point, p_image_list, p_image_none, scale);

    if (PNULL == p_multinum_info)
    {
        CTRL_WF_TRACE("[CTRLWF_MultiNum_Create]:p_multinum_info = pnull!");
        return handle;
    }
    else
    {
        handle = (MMI_HANDLE_T)p_multinum_info;
        p_multinum_info->handle = handle;
    }

    //store callback
    p_multinum_info->getdata_callback = getdata_callback;

    //set items ctrl id
    SetMultiNumItemsCtrlId(p_multinum_info, ctrl_id);


    CTRLWF_Common_GetImgInfoByPath(p_multinum_info->img_num[0], &img_info);

#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER)
    //create layer
    CTRLWF_Common_CreateLayer(&(p_multinum_info->layer), win_id, (img_info.image_width * IMG_NUM_MAX + WATCHFACE_CREATE_LAYER_MARGIN), (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN));
    if (p_multinum_info->layer.block_id == UILAYER_NULL_HANDLE)
    {
        CTRL_WF_TRACE("CreateCacheLayer [error].");
        SCI_MEMSET(p_multinum_info, 0, sizeof(WATCHFACE_MULTINUM_DATA_T));
        SCI_FREE(p_multinum_info);
        p_multinum_info = PNULL;
        return 0;
    }
    //set position
    UILAYER_SetLayerPosition(&(p_multinum_info->layer),
                            (p_multinum_info->center_point.x - (img_info.image_width * IMG_NUM_MAX + WATCHFACE_CREATE_LAYER_MARGIN) / 2),
                            (p_multinum_info->center_point.y - (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2)
                            );
#else
    MMK_GetWinLcdDevInfo(win_id, &(p_multinum_info->layer));
#endif
    //display num
    DisplayMultiNum(p_multinum_info, TRUE);

    if (0 != p_multinum_info->timer_id)
    {
        MMK_StopTimer(p_multinum_info->timer_id);
        p_multinum_info->timer_id = 0;
    }
    p_multinum_info->timer_id = MMK_CreateTimerCallback(timer_interval, UpdateMultiNum, (WATCHFACE_MULTINUM_DATA_T*)p_multinum_info, TRUE);

    return handle;
}

/*****************************************************************************/
//  Description : destory watch face multinum ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_MultiNum_Destory (MMI_HANDLE_T handle)
{
    BOOLEAN                     result = FALSE;
    WATCHFACE_MULTINUM_DATA_T*  p_multinum_info = PNULL;
    uint8                       index = 0;

    CTRL_WF_TRACE("[CTRLWF_MultiNum_Destory]:destory enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_multinum_info = (WATCHFACE_MULTINUM_DATA_T*)handle;

        if (handle == p_multinum_info->handle)
        {
            //stop timer
            MMK_StopTimer(p_multinum_info->timer_id);
            p_multinum_info->timer_id = 0;
            //destory ctrl
            for(index = IMG_NUM_ONE; index < IMG_NUM_MAX; index ++)
            {
                MMK_DestroyControl(p_multinum_info->ctrl_id[index]);
            }
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_multinum_info->layer));
            //release cache layer
            UILAYER_RELEASELAYER(&(p_multinum_info->layer));
#endif
            //free space
            SCI_MEMSET(p_multinum_info, 0, sizeof(WATCHFACE_MULTINUM_DATA_T));
            SCI_FREE(p_multinum_info);
            p_multinum_info = PNULL;
            result = TRUE;
        }
    }

    CTRL_WF_TRACE("[CTRLWF_MultiNum_Destory]:destory exit, result = %d", result);
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
PUBLIC BOOLEAN CTRLWF_MultiNum_StopTimer (MMI_HANDLE_T handle)
{
    BOOLEAN     result = FALSE;
    WATCHFACE_MULTINUM_DATA_T*  p_info = PNULL;

    CTRL_WF_TRACE("stop timer enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_MULTINUM_DATA_T*)handle;
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
PUBLIC void CTRLWF_MultiNum_RestartTimer (MMI_HANDLE_T handle)
{
    WATCHFACE_MULTINUM_DATA_T*      p_info = PNULL;

    CTRL_WF_TRACE("update enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_MULTINUM_DATA_T*)handle;
        if(PNULL == p_info)
        {
            CTRL_WF_TRACE("p_info is pnull");
            return;
        }
        if (handle == p_info->handle)
        {
            if (TRUE == MMK_IsFocusWin(p_info->win_id))
            {
                if (0 == p_info->timer_id)
                {
                    p_info->timer_id = MMK_CreateTimerCallback(p_info->timer_interval, UpdateMultiNum, (WATCHFACE_MULTINUM_DATA_T*)p_info, TRUE);
                }
                else
                {
                    CTRL_WF_TRACE("timer_id != 0");
                }
            }
        }
    }
}

