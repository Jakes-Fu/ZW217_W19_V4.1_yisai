/*****************************************************************************
** File Name:       ctrlwf_img.c                                             *
** Author:          yuming.yang                                              *
** Date:            08/13/2021                                               *
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
#include "ctrlwfimg.h"
#include "mmi_string.h"
#include "mmk_timer.h"

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
//  Description : create data
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_IMG_INFO_T* CreateImgData(MMI_WIN_ID_T              win_id,
                                            MMI_CTRL_ID_T               ctrl_id,
                                            GUI_POINT_T                 center_point,
                                            char*                       p_image_name,
                                            float                       scale
                                            )
{
    WATCHFACE_IMG_INFO_T*   p_info = PNULL;
    wchar                   img_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN] = {0};
    uint8                   wstr_len = 0;

    //alloc space
    p_info = SCI_ALLOC_APPZ(sizeof(WATCHFACE_IMG_INFO_T));
    if(PNULL == p_info)
    {
        CTRL_WF_TRACE("The p_info is PNULL");
        return PNULL;
    }
    SCI_MEMSET(p_info, 0, sizeof(WATCHFACE_IMG_INFO_T));

    //store info
    p_info->win_id = win_id;
    p_info->ctrl_id = ctrl_id;
    p_info->center_point = center_point;
    p_info->scale = scale;

    if(PNULL != p_image_name)
    {
        MMIAPICOM_StrToWstr((uint8 *)p_image_name, img_wstr);
        wstr_len = MMIAPICOM_Wstrlen(img_wstr);
        MMI_WSTRNCPY(p_info->img, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN,(const wchar*)img_wstr, wstr_len, wstr_len);
    }
    return p_info;
}


/*****************************************************************************/
//  Description : create watch face image ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position: 中心坐标
//             [In] p_image_name: 资源路径
//             [In] scale: 缩放比例
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLWF_Img_Create(MMI_WIN_ID_T         win_id,
                                               MMI_CTRL_ID_T        ctrl_id,
                                               GUI_POINT_T          center_point,
                                               char*                p_image_name,
                                               float                scale
                                               )
{
    MMI_HANDLE_T            handle = 0;
    WATCHFACE_IMG_INFO_T*   p_info = PNULL;
    GUIIMG_INFO_T           img_info = {0};

    if(PNULL == p_image_name)
    {
        CTRL_WF_TRACE("p_image_name = pnull!");
        return handle;
    }

    //create data
    p_info = CreateImgData(win_id, ctrl_id, center_point, p_image_name, scale);
    if(PNULL == p_info)
    {
        CTRL_WF_TRACE("p_bg_info is PNULL");
        return handle;
    }
    else
    {
        handle = (MMI_HANDLE_T)p_info;
        p_info->handle = handle;
    }


    CTRLWF_Common_GetImgInfoByPath(p_info->img, &img_info);

#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER)
    //create layer
    CTRLWF_Common_CreateLayer(&(p_info->layer), win_id, (img_info.image_width + WATCHFACE_CREATE_LAYER_MARGIN), (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN));
    if (p_info->layer.block_id == UILAYER_NULL_HANDLE)
    {
        CTRL_WF_TRACE("CreateCacheLayer [error].");
        SCI_MEMSET(p_info, 0, sizeof(WATCHFACE_IMG_INFO_T));
        SCI_FREE(p_info);
        p_info = PNULL;
        return 0;
    }
    //set position
    UILAYER_SetLayerPosition(&(p_info->layer),
                            (p_info->center_point.x - (img_info.image_width + WATCHFACE_CREATE_LAYER_MARGIN) / 2),
                            (p_info->center_point.y - (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2)
                            );
#else
    MMK_GetWinLcdDevInfo(win_id, &(p_info->layer));
    p_info->rect.left = (p_info->center_point.x - (img_info.image_width + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
    p_info->rect.top = (p_info->center_point.y - (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
    p_info->rect.right = (p_info->center_point.x + (img_info.image_width + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
    p_info->rect.bottom = (p_info->center_point.y + (img_info.image_height + WATCHFACE_CREATE_LAYER_MARGIN) / 2);
#endif

    CTRLWF_Common_CreateImg(win_id, ctrl_id, &p_info->rect, p_info->img, scale, &(p_info->layer));

    return handle;
}


/*****************************************************************************/
//  Description : destory watch face image ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Img_Destory (MMI_HANDLE_T handle)
{
    BOOLEAN                  result = FALSE;
    WATCHFACE_IMG_INFO_T*    p_info = PNULL;

    CTRL_WF_TRACE("destory enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_IMG_INFO_T*)handle;
        if (handle == p_info->handle)
        {
            //destory ctrl
            MMK_DestroyControl(p_info->ctrl_id);
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_info->layer));
            //release cache layer
            UILAYER_RELEASELAYER(&(p_info->layer));
#endif
            //free space
            SCI_MEMSET(p_info, 0, sizeof(WATCHFACE_IMG_INFO_T));
            SCI_FREE(p_info);
            result = TRUE;
        }
    }
    CTRL_WF_TRACE("destory exit, result = %d", result);
    return result;
}

