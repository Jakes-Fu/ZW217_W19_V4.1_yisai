/*****************************************************************************
** File Name:      guianim_internal.c                                        *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Create
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "guicommon.h"
#include "ctrlanim.h"
#include "guires.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get anim pointer by control handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLANIM_OBJ_T* GUIANIM_GetPtr(
                                      MMI_HANDLE_T  ctrl_handle
                                      )
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (GUIANIM_TypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLANIM_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_TypeOf(
                              CTRLBASE_OBJ_T     *ctrl_ptr
                              )
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_ANIM_TYPE);
}

/*****************************************************************************/
//  Description : get anim rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_GetAnimRect(
                                   CTRLANIM_OBJ_T   *anim_ctrl_ptr, //in:
                                   GUI_RECT_T       *rect_ptr       //in/out:
                                   )
{
    BOOLEAN         result = FALSE;
    BOOLEAN         is_border = TRUE;
    GUI_RECT_T      anim_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    if (!GUI_IsRectEmpty(base_ctrl_ptr->rect))
    {
        //anim is exist border
        if (GUI_BORDER_NONE == anim_ctrl_ptr->theme.border.type)
        {
            is_border = FALSE;
        }
    
        if (is_border)
        {
            anim_rect.left   = (int16)(base_ctrl_ptr->rect.left + anim_ctrl_ptr->theme.border.width);
            anim_rect.right  = (int16)(base_ctrl_ptr->rect.right - anim_ctrl_ptr->theme.border.width);
            anim_rect.top    = (int16)(base_ctrl_ptr->rect.top + anim_ctrl_ptr->theme.border.width);
            anim_rect.bottom = (int16)(base_ctrl_ptr->rect.bottom - anim_ctrl_ptr->theme.border.width);
        }
        else
        {
            anim_rect = base_ctrl_ptr->rect;
        }

        if (PNULL != rect_ptr)
        {
            *rect_ptr = anim_rect;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get anim is display horizontal
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_IsHorizontal(
                                    CTRLANIM_OBJ_T      *anim_ctrl_ptr
                                    )
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_rotate = FALSE;
    BOOLEAN     is_specify = FALSE;

	if (PNULL != anim_ctrl_ptr)
	{    
		CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

		//is rotate
		if ((GUIANIM_ROTATE_NONE != anim_ctrl_ptr->display_info.rotate_mode) || 
			((0 < anim_ctrl_ptr->display_info.rotate_angle) && (360 > anim_ctrl_ptr->display_info.rotate_angle)))
		{
			is_rotate = TRUE;
		}

		//is specify widht/height
		if ((0 < anim_ctrl_ptr->display_info.specify_width) && 
			(0 < anim_ctrl_ptr->display_info.specify_height))
		{
			is_specify = TRUE;
		}

		if ((PNULL != anim_ctrl_ptr) && 
			(anim_ctrl_ptr->is_auto_adapt_disp) &&  //auto adapt display
			(!anim_ctrl_ptr->display_info.is_zoom) &&   //not for zoom
			(!is_specify) && //not specify widht/height
			(!is_rotate))   //not rotate
		{
			//is landscape
			if (GUILCD_IsLandscape(base_ctrl_ptr->lcd_dev_info.lcd_id))
			{
				if (anim_ctrl_ptr->img_height > anim_ctrl_ptr->img_width)
				{
					result = TRUE;
				}
			}
			else
			{
				if (anim_ctrl_ptr->img_width > anim_ctrl_ptr->img_height)
				{
					result = TRUE;
				}
			}
		}
	}
    return (result);
}

/*****************************************************************************/
//  Description : get image width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_GetImgWidthAndHeight(
                                         BOOLEAN        is_miniature,   //in:
                                         BOOLEAN        is_src,         //in:
                                         uint16         *img_width_ptr, //in/out:may PNULL
                                         uint16         *img_height_ptr,//in/out:may PNULL
                                         CTRLANIM_OBJ_T *anim_ctrl_ptr  //in:
                                         )
{
    uint16  img_width = 0;
    uint16  img_height = 0;

    if (PNULL != anim_ctrl_ptr)
    {
        if ((anim_ctrl_ptr->display_info.is_zoom) &&
            (anim_ctrl_ptr->is_support_zoom))
        {
            if (PNULL != img_width_ptr)
            {
                *img_width_ptr  = anim_ctrl_ptr->zoom_dest.buf_width;
            }
            if (PNULL != img_height_ptr)
            {
                *img_height_ptr = anim_ctrl_ptr->zoom_dest.buf_height;
            }
        }
        else if (anim_ctrl_ptr->is_rotate)
        {
            if (PNULL != img_width_ptr)
            {
                *img_width_ptr  = anim_ctrl_ptr->rotate_dest.img_width;
            }
            if (PNULL != img_height_ptr)
            {
                *img_height_ptr = anim_ctrl_ptr->rotate_dest.img_height;
            }
        }
        else
        {
            if (is_miniature)
            {
                img_width  = anim_ctrl_ptr->miniature_width;
                img_height = anim_ctrl_ptr->miniature_height;
            }
            else if (anim_ctrl_ptr->is_decode_thumb)
            {
                img_width  = anim_ctrl_ptr->thumb_width;
                img_height = anim_ctrl_ptr->thumb_height;
            }
            else if (is_src)
            {
                img_width  = anim_ctrl_ptr->src_width;
                img_height = anim_ctrl_ptr->src_height;
            }
            else
            {
                img_width  = anim_ctrl_ptr->img_width;
                img_height = anim_ctrl_ptr->img_height;
            }

            if (GUIANIM_IsHorizontal(anim_ctrl_ptr))
            {
                if (PNULL != img_width_ptr)
                {
                    *img_width_ptr = img_height;
                }
                if (PNULL != img_height_ptr)
                {
                    *img_height_ptr = img_width;
                }
            }
            else
            {
                if (PNULL != img_width_ptr)
                {
                    *img_width_ptr = img_width;
                }
                if (PNULL != img_height_ptr)
                {
                    *img_height_ptr = img_height;
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : get decode target width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_GetTargetWidthAndHeight(
                                            BOOLEAN         is_for_zoom,        //in:is for zoom
                                            uint16          *target_width_ptr,  //in/out:
                                            uint16          *target_height_ptr, //in/out:
                                            CTRLANIM_OBJ_T  *anim_ctrl_ptr      //in
                                            )
{
    uint16          shrect_width = 0;
    uint16          shrect_height = 0;
    GUI_RECT_T      anim_rect = {0};

    if (is_for_zoom)
    {
        if ((anim_ctrl_ptr->theme.zoom_max_width * anim_ctrl_ptr->theme.zoom_max_height) < (anim_ctrl_ptr->img_width * anim_ctrl_ptr->img_height))
        {
            shrect_width  = anim_ctrl_ptr->theme.zoom_max_width;
            shrect_height = anim_ctrl_ptr->theme.zoom_max_height;
        }
        else
        {
            shrect_width  = anim_ctrl_ptr->img_width;
            shrect_height = anim_ctrl_ptr->img_height;
        }
    }
    else if (GUIANIM_TYPE_IMAGE_ICO == anim_ctrl_ptr->type)
    {
        shrect_width  = anim_ctrl_ptr->img_width;
        shrect_height = anim_ctrl_ptr->img_height;
    }
    else if ((0 < anim_ctrl_ptr->display_info.specify_width) &&
             (0 < anim_ctrl_ptr->display_info.specify_height))
    {
        //is specify width and height
        shrect_width  = anim_ctrl_ptr->display_info.specify_width;
        shrect_height = anim_ctrl_ptr->display_info.specify_height;
    }
    else
    {
        //get anim rect,except border
        GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect);
        
        //set show rect width and height
#ifdef GUIF_ANIM_ROTATE
        if (GUIANIM_IsRotateHorizontal(anim_ctrl_ptr))
        {
            shrect_width  = (uint16)(anim_rect.bottom - anim_rect.top + 1);
            shrect_height = (uint16)(anim_rect.right - anim_rect.left + 1);
        }
        else
#endif
        {
            shrect_width  = (uint16)(anim_rect.right - anim_rect.left + 1);
            shrect_height = (uint16)(anim_rect.bottom - anim_rect.top + 1);
        }
    }

    //adjust
    if (GUIANIM_IsHorizontal(anim_ctrl_ptr))
    {
        uint16  display_width = shrect_width;

        shrect_width  = shrect_height;
        shrect_height = display_width;
    }

    //set width and height
    if (PNULL != target_width_ptr)
    {
        *target_width_ptr  = shrect_width;
    }
    if (PNULL != target_height_ptr)
    {
        *target_height_ptr = shrect_height;
    }
}

/*****************************************************************************/
//  Description : get image show width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_GetShowWidthAndHeight(
                                          uint16          *show_width_ptr,  //in/out:
                                          uint16          *show_height_ptr, //in/out:
                                          CTRLANIM_OBJ_T  *anim_ctrl_ptr    //in
                                          )
{
    uint16          shrect_width = 0;
    uint16          shrect_height = 0;
    GUI_RECT_T      anim_rect = {0};

    //get anim rect,except border
    GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect);
    
    //set show rect width and height
#ifdef GUIF_ANIM_ROTATE
    if (GUIANIM_IsRotateHorizontal(anim_ctrl_ptr))
    {
        shrect_width  = (uint16)(anim_rect.bottom - anim_rect.top + 1);
        shrect_height = (uint16)(anim_rect.right - anim_rect.left + 1);
    }
    else
#endif
    {
        shrect_width  = (uint16)(anim_rect.right - anim_rect.left + 1);
        shrect_height = (uint16)(anim_rect.bottom - anim_rect.top + 1);
    }

    //adjust
    if (GUIANIM_IsHorizontal(anim_ctrl_ptr))
    {
        uint16  display_width = shrect_width;

        shrect_width  = shrect_height;
        shrect_height = display_width;
    }

    //set width and height
    if (PNULL != show_width_ptr)
    {
        *show_width_ptr  = shrect_width;
    }
    if (PNULL != show_height_ptr)
    {
        *show_height_ptr = shrect_height;
    }
}

/*****************************************************************************/
//  Description : 获取旋转后的图像数据
//  Global resource dependence : 
//  Author:hua.fang
//  Note: 
//  GUI内部接口，对iconlist公开
/*****************************************************************************/
PUBLIC void GUIANIM_GetRotateImage(
                                  MMI_HANDLE_T          ctrl_handle,
                                  GUIIMG_BITMAP_T       *img_bitmap_ptr,
                                  MMI_IMAGE_ID_T        *img_id_ptr
                                  )
{
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        // 如果有旋转数据，就取它，否则取普通数据
        if (PNULL != anim_ctrl_ptr->rotate_dest.buf_ptr)
        {
            if (PNULL != img_bitmap_ptr)
            {
                img_bitmap_ptr->bit_ptr = anim_ctrl_ptr->rotate_dest.buf_ptr + anim_ctrl_ptr->rotate_dest.buf_offset;
                img_bitmap_ptr->img_width = anim_ctrl_ptr->rotate_dest.img_width;
                img_bitmap_ptr->img_height = anim_ctrl_ptr->rotate_dest.img_height;
                img_bitmap_ptr->date_type = anim_ctrl_ptr->rotate_dest.data_type;
            }
        }
        else
        {
            GUIANIM_GetImgBuf(ctrl_handle, img_bitmap_ptr, img_id_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : 获取图像数据
//  Global resource dependence : 
//  Author:hua.fang
//  Note: 
//  GUI内部接口，对iconlist公开
/*****************************************************************************/
PUBLIC void GUIANIM_GetImgBuf(
                              MMI_HANDLE_T          ctrl_handle,
                              GUIIMG_BITMAP_T       *img_bitmap_ptr,
                              MMI_IMAGE_ID_T        *img_id_ptr
                              )
{
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        if (anim_ctrl_ptr->is_decoded)
        {
            if (PNULL != img_bitmap_ptr)
            {
                img_bitmap_ptr->bit_ptr = anim_ctrl_ptr->target_buffer + anim_ctrl_ptr->target_buf_offset;
                img_bitmap_ptr->img_width = anim_ctrl_ptr->img_width;
                img_bitmap_ptr->img_height = anim_ctrl_ptr->img_height;
                img_bitmap_ptr->date_type = anim_ctrl_ptr->target_data_type;
            }
        }
        else if (PNULL != img_id_ptr)
        {
            if (anim_ctrl_ptr->is_support)
            {
                *img_id_ptr = anim_ctrl_ptr->theme.default_img_id;
            }
            else
            {
                *img_id_ptr = anim_ctrl_ptr->theme.error_bg.img_id;
            }
        }
    }
}

/*****************************************************************************/
//  Description : 根据控件handle获得图片的宽或者高
//  Global resource dependence : 
//  Author:hua.fang
//  Note: 
//  GUI内部接口，对iconlist公开
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_GetSrcImgWidthHeight(
                                            MMI_HANDLE_T     ctrl_handle,//in:
                                            uint16           *width_ptr, //in/out:may PNULL
                                            uint16           *height_ptr //in/out:may PNULL
                                            )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        if (PNULL != width_ptr)
        {
            *width_ptr = anim_ctrl_ptr->src_width;
        }

        if (PNULL != height_ptr)
        {
            *height_ptr = anim_ctrl_ptr->src_height;
        }

        result = TRUE;
    }

    return result;
}


