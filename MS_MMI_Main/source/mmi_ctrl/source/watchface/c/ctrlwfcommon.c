/*****************************************************************************
** File Name:       ctrlwfcommon.c                                           *
** Author:          yuming.yang                                              *
** Date:            08/13/2021                                               *
** Copyright:                                                                *
** Description:    This file is used to define watch face ctrl common api    *
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
#include "ctrlanim.h"
#include "mmk_type.h"
#include "img_dec_interface.h"
#include "mmi_string.h"
#include "guictrl_api.h"
#include "ctrlwfcommon.h"
#include "guilcd.h"


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
//  Description : set anim param
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E SetAnimParam(MMI_CTRL_ID_T        ctrl_id,
                                           GUI_RECT_T*          disp_rect,
                                           wchar*               p_image_name,
                                           GUIANIM_CTRL_INFO_T* p_control_info,
                                           float                scale
                                           )
{
    GUIANIM_FILE_INFO_T     file_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    GUIANIM_RESULT_E        anim_res = GUIANIM_RESULT_MAX;
    GUIIMG_INFO_T           img_info = {0};
//    GUI_RECT_T              display_rect = {0};

    if(PNULL == p_image_name)
    {
        CTRL_WF_TRACE("image_name = pnull!");
        return GUIANIM_RESULT_EMPTY;
    }

    //get file info
//    CTRLWF_Common_GetImgInfoByPath(p_image_name, &img_info);

    //get display rect
//    CTRLWF_Common_GetDisplayRect(center_point, img_info, &display_rect, scale);

    //set file info
    file_info.full_path_wstr_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(WATCHFACE_IMAGE_FULL_PATH_MAX_LEN + 1));
    if (PNULL == file_info.full_path_wstr_ptr)
    {
        CTRL_WF_TRACE("full_path_wstr_ptr = pnull!");
        return GUIANIM_RESULT_EMPTY;
    }
    SCI_MEMSET(file_info.full_path_wstr_ptr,0,(sizeof(wchar)*(WATCHFACE_IMAGE_FULL_PATH_MAX_LEN + 1)));

    file_info.full_path_wstr_len = MMIAPICOM_Wstrlen(p_image_name);
    MMI_WSTRNCPY(file_info.full_path_wstr_ptr,sizeof(wchar)*(file_info.full_path_wstr_len+1),
                p_image_name,file_info.full_path_wstr_len,
                file_info.full_path_wstr_len);

    //set display info
    display_info.is_update = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_disp_one_frame = TRUE;
    display_info.is_crop_rect = TRUE;
    display_info.is_syn_decode = TRUE;
    display_info.is_handle_transparent = TRUE;
    display_info.is_auto_zoom_in = TRUE;
    display_info.is_zoom = (1 == scale) ? FALSE : TRUE;

    //set anim rect
    GUIAPICTRL_SetRect(ctrl_id, disp_rect);

    //set bg anim
    anim_res = CTRLANIM_SetParam(p_control_info, PNULL, &file_info, &display_info);

    if (PNULL != file_info.full_path_wstr_ptr)
    {
        SCI_FREE(file_info.full_path_wstr_ptr);
    }

    CTRL_WF_TRACE(" res of GUIANIM_SetParam = %d!", anim_res);
    return anim_res;
}



/*****************************************************************************/
//  Description : calculate display rect by center point and img info
//  Parameter: [In] center_point
//             [In] img_info
//             [Out] p_rect
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Common_GetDisplayRect(GUI_POINT_T center_point,GUIIMG_INFO_T img_info, GUI_RECT_T* p_rect, float scale)
{
    uint16      img_width = 0;
    uint16      img_height = 0;

    if(PNULL == p_rect)
    {
        CTRL_WF_TRACE("rect_ptr = pnull!");
        return FALSE;
    }

    img_width = img_info.image_width * scale;
    img_height = img_info.image_height * scale;

    CTRL_WF_TRACE("center_point = [%d, %d], scale = %f, img_width = %d, img_height = %d", center_point.x, center_point.y, scale, img_width, img_height);
    p_rect->left = center_point.x - img_width / 2;
    p_rect->top  = center_point.y - img_height / 2;
    p_rect->right = center_point.x + img_width / 2;
    p_rect->bottom = center_point.y + img_height / 2;

    return TRUE;
}


/*****************************************************************************/
//  Description : get image width and height by image path
//  Parameter: [In] p_full_path_wstr: 文件全路径
//             [Out] p_img_info: img信息
//             [Return] 成功或失败
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Common_GetImgInfoByPath(wchar* p_full_path_wstr,
                                                                 GUIIMG_INFO_T* p_img_info
                                                                 )
{
    BOOLEAN             result = TRUE;
    IMG_DEC_RET_E       get_result = IMG_DEC_RET_SUCCESS;
    IMG_DEC_SRC_T       dec_src = {0};
    IMG_DEC_INFO_T      dec_info = {0};

    if(PNULL == p_img_info || PNULL == p_full_path_wstr)
    {
        CTRL_WF_TRACE("full_path_wstr_ptr = pnull or img_info_ptr = pnull!");
        return FALSE;
    }

    //set decode src info
    dec_src.file_name_ptr = p_full_path_wstr;

    //get image info
    get_result = IMG_DEC_GetInfo(&dec_src, &dec_info);
    CTRL_WF_TRACE("get_result = %d, img_type = %d", get_result, dec_info.img_type);

    if (IMG_DEC_RET_SUCCESS == get_result)
    {
        //set image info
        switch (dec_info.img_type)
        {
            case IMG_DEC_TYPE_BMP:
            {
                p_img_info->image_width  = dec_info.img_detail_info.bmp_info.img_width;
                p_img_info->image_height = dec_info.img_detail_info.bmp_info.img_height;
                break;
            }
            case IMG_DEC_TYPE_WBMP:
            {
                p_img_info->image_width  = dec_info.img_detail_info.wbmp_info.img_width;
                p_img_info->image_height = dec_info.img_detail_info.wbmp_info.img_height;
                break;
            }
            case IMG_DEC_TYPE_JPEG:
            {
                p_img_info->image_width  = dec_info.img_detail_info.jpeg_info.img_width;
                p_img_info->image_height = dec_info.img_detail_info.jpeg_info.img_heigth;
                break;
            }
            case IMG_DEC_TYPE_PNG:
            {
                p_img_info->image_width  = dec_info.img_detail_info.png_info.img_width;
                p_img_info->image_height = dec_info.img_detail_info.png_info.img_height;
                break;
            }
            case IMG_DEC_TYPE_GIF:
            {
                p_img_info->image_width  = dec_info.img_detail_info.gif_info.img_width;
                p_img_info->image_height = dec_info.img_detail_info.gif_info.img_height;
                break;
            }
            case IMG_DEC_TYPE_UNKNOWN:
                break;

            default:
                result = FALSE;
                break;
        }
    }
    else
    {
        result = FALSE;
    }
    return result;
}


/*****************************************************************************/
//  Description : create image ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] disp_rect: 绘制区域
//             [In] p_image_name: 资源路径
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLWF_Common_CreateImg(MMI_WIN_ID_T         win_id,
                                                           MMI_CTRL_ID_T        ctrl_id,
                                                           GUI_RECT_T*          disp_rect,
                                                           wchar*               p_image_name,
                                                           float                scale,
                                                           GUI_LCD_DEV_INFO*    p_lcd_info
                                                           )
{
    MMI_HANDLE_T            handle = 0;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    MMI_CONTROL_CREATE_T    anim_create = {0};
    CTRLANIM_OBJ_T*         anim_ctrl_ptr = PNULL;
    GUIANIM_RESULT_E        anim_res = GUIANIM_RESULT_MAX;

    if(PNULL == p_image_name)
    {
        CTRL_WF_TRACE("image_name = pnull!");
        return handle;
    }

    if(PNULL == p_lcd_info)
    {
        CTRL_WF_TRACE("p_lcd_info = pnull!");
        return handle;
    }

    //create anim
    MMK_DestroyControl(ctrl_id);
    anim_create.guid = SPRD_GUI_ANIM_ID;
    anim_create.ctrl_id = ctrl_id;
    anim_create.init_data_ptr = PNULL;
    anim_create.parent_win_handle = win_id;
    anim_ctrl_ptr = (CTRLANIM_OBJ_T*)MMK_CreateControl(&anim_create);

    if (PNULL != anim_ctrl_ptr)
    {
        handle = anim_ctrl_ptr->base_ctrl.handle;
        //set control info
        control_info.is_ctrl_id = FALSE;
        control_info.ctrl_ptr = anim_ctrl_ptr;
    }

    //set draw to which layer
    //if (0 != p_lcd_info->block_id)
    {
        GUIAPICTRL_SetLcdDevInfo(ctrl_id, p_lcd_info);
    }

    //set bg anim
    anim_res = SetAnimParam(ctrl_id, disp_rect, p_image_name, &control_info, scale);

    if(GUIANIM_RESULT_SUCC != anim_res)
    {
        MMK_DestroyControl(handle);
        handle = 0;
    }
    return handle;
}


/*****************************************************************************/
//  Description : update image ctrl
//  Parameter: [In] ctrl_id
//             [In] disp_rect: 绘制区域
//             [In] p_image_name: 资源路径
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Common_UpdateImg(MMI_CTRL_ID_T        ctrl_id,
                                                       GUI_RECT_T*          disp_rect,
                                                       wchar*               p_image_name,
                                                       float                scale,
                                                       GUI_LCD_DEV_INFO*    p_lcd_info
                                                       )
{
    BOOLEAN                 result = FALSE;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_RESULT_E        anim_res = GUIANIM_RESULT_MAX;

    if(PNULL == p_image_name)
    {
        CTRL_WF_TRACE("image_name = pnull!");
        return result;
    }

    //set control info
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;
    if(p_lcd_info != PNULL)
    {
        GUIAPICTRL_SetLcdDevInfo(ctrl_id, p_lcd_info);
    }
    //set bg anim
    anim_res = SetAnimParam(ctrl_id, disp_rect, p_image_name, &control_info, scale);

    if(GUIANIM_RESULT_SUCC == anim_res)
    {
        result = TRUE;
    }
    return result;
}


/*****************************************************************************/
//  Description : update image disp
//  Parameter: [In] ctrl_id
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_Common_DisplayImg(MMI_CTRL_ID_T ctrl_id, GUI_LCD_DEV_INFO*    p_lcd_info)
{

    CTRL_WF_TRACE("ctrl_id[%x], lcd_block[%x] ", ctrl_id, p_lcd_info->block_id);
    if(p_lcd_info != PNULL)
    {
        GUIAPICTRL_SetLcdDevInfo(ctrl_id, p_lcd_info);
    }
    //CTRLANIM_UpdateDisplay(ctrl_id);
    GUIANIM_Display(TRUE,TRUE, GUIANIM_GetPtr(ctrl_id));
    return ;
}


/*****************************************************************************/
//  Description : create layer
//  Parameter: [In] win_id
//             [In] width: layer宽度
//             [In] height: layer高度
//             [Out] p_lcd_dev: layer info
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_Common_CreateLayer(GUI_LCD_DEV_INFO*     p_lcd_dev,
                                                      MMI_WIN_ID_T          win_id,
                                                      uint16                width,
                                                      uint16                height
                                                      )
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_CREATE_T    create_info = {0};

    CTRL_WF_TRACE("Enter, width = %d, height = %d", width, height);
    if (PNULL == p_lcd_dev)
    {
        CTRL_WF_TRACE("p_lcd_dev = pnull!");
        return;
    }

    if (p_lcd_dev->block_id != UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(p_lcd_dev);
    }

    p_lcd_dev->lcd_id = GUI_MAIN_LCD_ID;
    p_lcd_dev->block_id = UILAYER_NULL_HANDLE;

    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = win_id;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = width;
    create_info.height = height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = TRUE;

    if(UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info, p_lcd_dev))
    {
        UILAYER_APPEND_BLT_T append_layer = {0};
        append_layer.lcd_dev_info.lcd_id = p_lcd_dev->lcd_id;
        append_layer.lcd_dev_info.block_id = p_lcd_dev->block_id;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);

        UILAYER_Clear(p_lcd_dev);
    }

#endif
}

/*****************************************************************************/
//  Description : append watch face layer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Common_AppendLayer(MMI_HANDLE_T handle)
{
    BOOLEAN                    result = FALSE;
    WATCHFACE_COMMON_ITEM_T*   p_info = PNULL;

    CTRL_WF_TRACE("append enter, handle = %d", handle);
#ifdef UI_MULTILAYER_SUPPORT
    if (0 != handle)
    {
        p_info = (WATCHFACE_COMMON_ITEM_T*)handle;
        if (handle == p_info->handle)
        {
            //append layer
            if (!UILAYER_IsBltLayer(&(p_info->layer)))
            {
                UILAYER_APPEND_BLT_T append = {0};
                append.layer_level = UILAYER_LEVEL_NORMAL;
                append.lcd_dev_info = p_info->layer;
                UILAYER_AppendBltLayer(&append);
                SCI_TRACE_LOW("[SFL]CTRLWF_Common_AppendLayer block_id:%d", p_info->layer.block_id);
                result = TRUE;
            }
        }
    }
#endif
    CTRL_WF_TRACE("append exit, result = %d", result);
    return result;
}

/*****************************************************************************/
//  Description : remove watch face layer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Common_RemoveLayer (MMI_HANDLE_T handle)
{
    BOOLEAN                    result = FALSE;
    WATCHFACE_COMMON_ITEM_T*   p_info = PNULL;

    CTRL_WF_TRACE("remove enter, handle = %d", handle);
#ifdef UI_MULTILAYER_SUPPORT
    if (0 != handle)
    {
        p_info = (WATCHFACE_COMMON_ITEM_T*)handle;
        if (handle == p_info->handle)
        {
            //remove cache layer
            UILAYER_RemoveBltLayer(&(p_info->layer));
            result = TRUE;
        }
    }
#endif
    CTRL_WF_TRACE("remove exit, result = %d", result);
    return result;
}

