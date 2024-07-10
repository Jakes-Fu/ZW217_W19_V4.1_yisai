#ifdef GUIF_ANIM_ZOOM
/*****************************************************************************
** File Name:      guianim_zoom.c                                            *
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
#include "mmi_gui_trc.h"
#include "sci_types.h"
#include "mmk_type.h"
#include "ctrlanim.h"

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
/*****************************************************************************/
//  Description : zoom in/out image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E ZoomImage(
                                 uint16         target_width,   //in:缩放后图片宽
                                 uint16         target_height,  //in:缩放后图片高
                                 GUI_RECT_T     *clip_rect_ptr, //in:原图的clip rect
                                 CTRLANIM_OBJ_T *anim_ctrl_ptr  //in
                                 );

/*****************************************************************************/
//  Description : get zoom info,include image width/height after zoom and ratio
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetZoomInfo(
                          BOOLEAN           is_zoom_in,         //in:
                          uint16            *target_width_ptr,  //in/out:
                          uint16            *target_height_ptr, //in/out:
                          CTRLANIM_OBJ_T    *anim_ctrl_ptr,     //in:
                          MMITHEME_RATIO_T  *ratio_ptr          //in/out:
                          );

/*****************************************************************************/
//  Description : get image clip rect after zoom
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetCurClipRect(
                                uint16              shrect_width,   //in:
                                uint16              shrect_height,  //in:
                                uint16              target_width,   //in:
                                uint16              target_height,  //in:
                                CTRLANIM_OBJ_T      *anim_ctrl_ptr, //in:
                                MMITHEME_RATIO_T    *zoom_ratio_ptr //in:
                                );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : auto zoom image to display in anim rect
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_AutoZoom(
                                CTRLANIM_OBJ_T  *anim_ctrl_ptr  //in
                                )
{
    BOOLEAN             result = TRUE;
    uint16              shrect_width = 0;
    uint16              shrect_height = 0;
    GUI_RECT_T          clip_rect = {0};
    GUI_RECT_T          anim_rect = {0};
    GUIANIM_RESULT_E    zoom_result = GUIANIM_RESULT_FAIL;

    if ((anim_ctrl_ptr->display_info.is_zoom) &&
        (anim_ctrl_ptr->is_support_zoom))
    {
        //get show width,height
        GUIANIM_GetShowWidthAndHeight(&shrect_width,
            &shrect_height,
            anim_ctrl_ptr);

        //adjust show width and height
        if (!anim_ctrl_ptr->display_info.is_auto_zoom_in)
        {
            if (anim_ctrl_ptr->img_width < shrect_width)
            {
                shrect_width = anim_ctrl_ptr->img_width;
            }
            if (anim_ctrl_ptr->img_height < shrect_height)
            {
                shrect_height = anim_ctrl_ptr->img_height;
            }
        }

        //set clip rect
        clip_rect.right  = (int16)(anim_ctrl_ptr->img_width - 1);
        clip_rect.bottom = (int16)(anim_ctrl_ptr->img_height - 1);

        //zoom image
        zoom_result = ZoomImage(shrect_width,shrect_height,&clip_rect,anim_ctrl_ptr);
        if (GUIANIM_RESULT_SUCC == zoom_result)
        {
            //set zoom min width and height
            anim_ctrl_ptr->zoom_min_width  = anim_ctrl_ptr->zoom_dest.buf_width;
            anim_ctrl_ptr->zoom_min_height = anim_ctrl_ptr->zoom_dest.buf_height;

            //set zoom current image width height
            anim_ctrl_ptr->zoom_dest.cur_img_width  = anim_ctrl_ptr->zoom_dest.buf_width;
            anim_ctrl_ptr->zoom_dest.cur_img_height = anim_ctrl_ptr->zoom_dest.buf_height;

            //set current image clip rect
            anim_ctrl_ptr->zoom_dest.cur_clip_rect.left   = 0;
            anim_ctrl_ptr->zoom_dest.cur_clip_rect.top    = 0;
            anim_ctrl_ptr->zoom_dest.cur_clip_rect.right  = (int16)(anim_ctrl_ptr->zoom_dest.cur_img_width - 1);
            anim_ctrl_ptr->zoom_dest.cur_clip_rect.bottom = (int16)(anim_ctrl_ptr->zoom_dest.cur_img_height - 1);

            //set support zoom
            anim_ctrl_ptr->is_support_zoom = TRUE;

            result = TRUE;
        }
        else
        {
            //free buffer
            SCI_FREE(anim_ctrl_ptr->zoom_dest.buf_ptr);
            anim_ctrl_ptr->zoom_dest.buf_size = 0;
            anim_ctrl_ptr->zoom_dest.buf_offset = 0;

            //src size not support zoom,display src image
            if (GUIANIM_RESULT_NO_SUPPORT == zoom_result)
            {
                result = TRUE;
            }
            else
            {
                if (GUI_BG_COLOR == anim_ctrl_ptr->theme.error_bg.bg_type)
                {
                    //set zoom current image width height
                    if (shrect_width / anim_ctrl_ptr->img_width > shrect_height/anim_ctrl_ptr->img_height)
                    {
                        anim_ctrl_ptr->zoom_dest.cur_img_height = shrect_height;
                        anim_ctrl_ptr->zoom_dest.cur_img_width  = (uint16)(anim_ctrl_ptr->img_width * shrect_height/anim_ctrl_ptr->img_height);
                    }
                    else
                    {
                        anim_ctrl_ptr->zoom_dest.cur_img_width  = shrect_width;
                        anim_ctrl_ptr->zoom_dest.cur_img_height = (uint16)(anim_ctrl_ptr->img_height * shrect_width/anim_ctrl_ptr->img_width);
                    }

                    //get anim rect
                    GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect);

                    anim_ctrl_ptr->display_x = anim_rect.left;
                    anim_ctrl_ptr->display_y = anim_rect.top;

                    //adjust display position
                    CTRLANIM_AdjustDisplayPosition(&anim_ctrl_ptr->display_x,
                        &anim_ctrl_ptr->display_y,
                        anim_ctrl_ptr->zoom_dest.cur_img_width,
                        anim_ctrl_ptr->zoom_dest.cur_img_height,
                        anim_rect,
                        anim_ctrl_ptr->display_info.align_style);
                }
            }

            //set not support zoom
            anim_ctrl_ptr->is_support_zoom = FALSE;
        }
    }

    return (result);
}
/*****************************************************************************/
//  Description : zoomin/zoomout image to utmost
//  Global resource dependence : 
//  Author:kevin.lou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_ZoomUtmost(
                                MMI_HANDLE_T    ctrl_handle    //in:  
                                )
{
    BOOLEAN             result          = FALSE;
    uint16              target_width    = 0;
    uint16              target_height   = 0;
    uint16              shrect_width    = 0;
    uint16              shrect_height   = 0;
    GUI_RECT_T          cur_clip_rect   = {0};
    GUI_RECT_T          src_clip_rect   = {0};
    GUI_RECT_T          anim_rect       = {0};
    CTRLANIM_OBJ_T      *anim_ctrl_ptr  = PNULL;
    CTRLBASE_OBJ_T      *base_ctrl_ptr  = PNULL;
    MMITHEME_RATIO_T    zoom_ratio      = {0};
    GUIANIM_RESULT_E    zoom_result     = GUIANIM_RESULT_FAIL;
    //BOOLEAN             is_zoomin_utmost = TRUE;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    
    if ((PNULL != anim_ctrl_ptr) &&
        (anim_ctrl_ptr->display_info.is_zoom) &&
        (anim_ctrl_ptr->is_support) &&
        (anim_ctrl_ptr->is_support_zoom) &&
        (anim_ctrl_ptr->is_decoded))
    {
        base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

        //get zoom info,include image width/height after zoom and ratio
        //result = GetZoomInfo(is_zoom_in,&target_width,&target_height,anim_ctrl_ptr,&zoom_ratio);
        {
            uint16 width_max,height_max,width_min,height_min;
            
            result = CTRLANIM_GetZoomWidthRange(base_ctrl_ptr->handle,&width_min,&width_max,&height_min,&height_max);
            
            if (width_max > anim_ctrl_ptr->zoom_dest.cur_img_width)
            {
                //is_zoomin_utmost    = TRUE;
                target_width        = width_max;
                target_height       = height_max;
                zoom_ratio.num1     = width_max;
                zoom_ratio.num2     = anim_ctrl_ptr->zoom_dest.cur_img_width;
            }
            else
            { 
                //is_zoomin_utmost    = FALSE;
                target_width        = width_min;
                target_height       = height_min;
                zoom_ratio.num1     = width_min;
                zoom_ratio.num2     = anim_ctrl_ptr->zoom_dest.cur_img_width;
            }            
        }
        ///
        
        //may zoom
        if (result)
        {
            //get show width,height
            //获得图片显示的宽和高
            GUIANIM_GetShowWidthAndHeight(&shrect_width,
                &shrect_height,
                anim_ctrl_ptr);
            if (target_width < shrect_width)
            {
                shrect_width = target_width;
            }
            if (target_height < shrect_height)
            {
                shrect_height = target_height;
            }

            //get image clip rect after zoom
            cur_clip_rect = GetCurClipRect(shrect_width,
                                shrect_height,
                                target_width,
                                target_height,
                                anim_ctrl_ptr,
                                &zoom_ratio);

            //set src clip rect
            src_clip_rect.left   = (int16)(cur_clip_rect.left * anim_ctrl_ptr->img_width / target_width);
            src_clip_rect.top    = (int16)(cur_clip_rect.top * anim_ctrl_ptr->img_height / target_height);
            src_clip_rect.right  = (int16)(src_clip_rect.left + shrect_width * anim_ctrl_ptr->img_width / target_width - 1);
            src_clip_rect.bottom = (int16)(src_clip_rect.top + shrect_height * anim_ctrl_ptr->img_height / target_height - 1);

            //zoom image
            zoom_result = ZoomImage(shrect_width,shrect_height,&src_clip_rect,anim_ctrl_ptr);

            if (GUIANIM_RESULT_SUCC == zoom_result)
            {
                //get anim rect
                GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect);

                anim_ctrl_ptr->display_x = anim_rect.left;
                anim_ctrl_ptr->display_y = anim_rect.top;

                //adjust display position
                CTRLANIM_AdjustDisplayPosition(&anim_ctrl_ptr->display_x,
                    &anim_ctrl_ptr->display_y,
                    anim_ctrl_ptr->zoom_dest.buf_width,
                    anim_ctrl_ptr->zoom_dest.buf_height,
                    anim_rect,
                    anim_ctrl_ptr->display_info.align_style);

                //set adjust display pos flag
                anim_ctrl_ptr->is_adjust_disp_pos = TRUE;

                //set current image width and height
                anim_ctrl_ptr->zoom_dest.cur_img_width  = target_width;
                anim_ctrl_ptr->zoom_dest.cur_img_height = target_height;

                //set current image clip rect
                anim_ctrl_ptr->zoom_dest.cur_clip_rect = cur_clip_rect;
                
                //display
                GUIANIM_Display(FALSE,FALSE,anim_ctrl_ptr);                

                result = TRUE;
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"GUIANIM_ZoomUtmost:ZoomUtmost fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_ZOOM_312_112_2_18_3_15_5_48,(uint8*)"");
    }

    return (result);
}

/*****************************************************************************/
//  Description : zoom in/out image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E ZoomImage(
                                 uint16         target_width,   //in:缩放后图片宽
                                 uint16         target_height,  //in:缩放后图片高
                                 GUI_RECT_T     *clip_rect_ptr, //in:原图的clip rect
                                 CTRLANIM_OBJ_T *anim_ctrl_ptr  //in
                                 )
{
    uint16                  shrect_width = 0;
    uint16                  shrect_height = 0;
    GUI_RECT_T              anim_rect = {0};
    GUIANIM_RESULT_E        result = GUIANIM_RESULT_FAIL;
    GUIANIM_ZOOM_INPUT_T    zoom_input = {0};
    GUIANIM_ZOOM_OUTPUT_T   zoom_output = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != clip_rect_ptr); /*assert verified*/
    if (PNULL == clip_rect_ptr)
    {
        return GUIANIM_RESULT_WRONG_PARAM;
    }

    //set src
    zoom_input.src_buf_ptr    = anim_ctrl_ptr->target_buffer + anim_ctrl_ptr->target_buf_offset;
    zoom_input.src_buf_size   = anim_ctrl_ptr->target_buffer_size;
    zoom_input.src_img_width  = anim_ctrl_ptr->img_width;
    zoom_input.src_img_height = anim_ctrl_ptr->img_height;
    zoom_input.src_data_type  = anim_ctrl_ptr->target_data_type;

    //set dest width height
    zoom_input.dest_img_width  = target_width;
    zoom_input.dest_img_height = target_height;

    //get anim rect,except border
    GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect);

    //set show rect width and height
    shrect_width  = (uint16)(anim_rect.right - anim_rect.left + 1);
    shrect_height = (uint16)(anim_rect.bottom - anim_rect.top + 1);

    //alloc target buffer
    if (PNULL == anim_ctrl_ptr->zoom_dest.buf_ptr)
    {
        if (GUIIMG_DATE_TYPE_ARGB888 == zoom_input.src_data_type)
        {
            anim_ctrl_ptr->zoom_dest.buf_size = (uint32)(shrect_width*shrect_height*4 + GUIANIM_BUF_ALIGN);
        }
        else
        {
            anim_ctrl_ptr->zoom_dest.buf_size = (uint32)(shrect_width*shrect_height*2 + GUIANIM_BUF_ALIGN);
        }
        anim_ctrl_ptr->zoom_dest.buf_ptr  = SCI_ALLOCA(anim_ctrl_ptr->zoom_dest.buf_size);
        if (NULL != anim_ctrl_ptr->zoom_dest.buf_ptr)
        {
            SCI_MEMSET(anim_ctrl_ptr->zoom_dest.buf_ptr,0,anim_ctrl_ptr->zoom_dest.buf_size);            
        }
        else
        {
            return GUIANIM_RESULT_LACK_MEMORY;
        }
    }
    else
    {
        SCI_PASSERT((shrect_width*shrect_height >= target_width*target_height),("ZoomImage:%d*%d < %d*%d",shrect_width,shrect_height,target_width,target_height)); /*assert verified*/
    }

    if (PNULL != anim_ctrl_ptr->zoom_dest.buf_ptr)
    {
        //set target buffer
        zoom_input.dest_buf_ptr  = anim_ctrl_ptr->zoom_dest.buf_ptr;
        zoom_input.dest_buf_size = anim_ctrl_ptr->zoom_dest.buf_size;

        //set clip rect
        zoom_input.clip_rect = *clip_rect_ptr;

        //zoom image
        result = GUIANIM_ZoomImage(&zoom_input,&zoom_output);
        if (GUIANIM_RESULT_SUCC == result)
        {
            //set zoom buffer width and height
            anim_ctrl_ptr->zoom_dest.buf_width  = zoom_output.img_width;
            anim_ctrl_ptr->zoom_dest.buf_height = zoom_output.img_height;

            //set zoom buffer offset
            anim_ctrl_ptr->zoom_dest.buf_offset = zoom_output.dest_buf_offset;

            //set zoom data type
            anim_ctrl_ptr->zoom_dest.data_type = zoom_output.data_type;
        }
    }
    else
    {
        SCI_TRACE_LOW("GUIANIM:ZoomImage:no zoom buffer!");
    }
    return (result);
}

/*****************************************************************************/
//  Description : get zoom width range
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetZoomWidthRange(
                                         MMI_HANDLE_T   ctrl_handle,    //in:
                                         uint16         *width_min_ptr, //in/out:may PNULL
                                         uint16         *width_max_ptr, //in/out:may PNULL
                                         uint16         *height_min_ptr,//in/out:may PNULL
                                         uint16         *height_max_ptr //in/out:may PNULL
                                         )
{
    BOOLEAN         result = FALSE;
    uint16          min_range = 0;
    uint16          max_range = 0;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if ((PNULL != anim_ctrl_ptr) &&
        (anim_ctrl_ptr->display_info.is_zoom) && 
        (anim_ctrl_ptr->is_support) && 
        (anim_ctrl_ptr->is_decoded))
    {
        if (anim_ctrl_ptr->is_support_zoom)
        {
            //get zoom range
            if (GUIANIM_ZoomRange(&min_range,&max_range))
            {
                //set min width
                if (PNULL != width_min_ptr)
                {
                    *width_min_ptr = anim_ctrl_ptr->zoom_min_width;
                    SCI_ASSERT((anim_ctrl_ptr->img_width/anim_ctrl_ptr->zoom_min_width) <= min_range); /*assert verified*/
                }

                //set min height
                if (PNULL != height_min_ptr)
                {
                    *height_min_ptr = anim_ctrl_ptr->zoom_min_height;
                    SCI_ASSERT((anim_ctrl_ptr->img_height/anim_ctrl_ptr->zoom_min_height) <= min_range); /*assert verified*/
                }

                //set max width
                if (PNULL != width_max_ptr)
                {
                    *width_max_ptr = (uint16)(anim_ctrl_ptr->img_width*max_range);
                }

                //set max height
                if (PNULL != height_max_ptr)
                {
                    *height_max_ptr = (uint16)(anim_ctrl_ptr->img_height*max_range);
                }
        
                result = TRUE;
            }
        }
        else
        {
            //set min width
            if (PNULL != width_min_ptr)
            {
                *width_min_ptr = anim_ctrl_ptr->img_width;
            }

            //set min height
            if (PNULL != height_min_ptr)
            {
                *height_min_ptr = anim_ctrl_ptr->img_height;
            }

            //set max width
            if (PNULL != width_max_ptr)
            {
                *width_max_ptr = anim_ctrl_ptr->img_width;
            }

            //set max height
            if (PNULL != height_max_ptr)
            {
                *height_max_ptr = anim_ctrl_ptr->img_height;
            }

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get zoom clip rect to current image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetZoomClipRect(
                                       MMI_HANDLE_T     ctrl_handle,//in:
                                       GUI_RECT_T       *rect_ptr   //in/out
                                       )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if ((PNULL != anim_ctrl_ptr) &&
        (anim_ctrl_ptr->display_info.is_zoom))
    {
        if (PNULL != rect_ptr)
        {
            if (anim_ctrl_ptr->is_support_zoom)
            {
                *rect_ptr = anim_ctrl_ptr->zoom_dest.cur_clip_rect;
            }
            else
            {
                rect_ptr->left   = 0;
                rect_ptr->top    = 0;
                rect_ptr->right  = (int16)(anim_ctrl_ptr->img_width - 1);
                rect_ptr->bottom = (int16)(anim_ctrl_ptr->img_height - 1);
            }
        }
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle zoom in or out
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_Zoom(
                            MMI_HANDLE_T    ctrl_handle,
                            BOOLEAN         is_zoom_in,
                            BOOLEAN         is_update
                            )
{
    BOOLEAN             result = FALSE;
    uint16              target_width = 0;
    uint16              target_height = 0;
    uint16              shrect_width = 0;
    uint16              shrect_height = 0;
    GUI_RECT_T          cur_clip_rect = {0};
    GUI_RECT_T          src_clip_rect = {0};
    GUI_RECT_T          anim_rect = {0};
    CTRLANIM_OBJ_T      *anim_ctrl_ptr = PNULL;
    MMITHEME_RATIO_T    zoom_ratio = {0};
    GUIANIM_RESULT_E    zoom_result = GUIANIM_RESULT_FAIL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if ((PNULL != anim_ctrl_ptr) &&
        (anim_ctrl_ptr->display_info.is_zoom) &&
        (anim_ctrl_ptr->is_support) &&
        (anim_ctrl_ptr->is_support_zoom) &&
        (anim_ctrl_ptr->is_decoded))
    {
        //get zoom info,include image width/height after zoom and ratio
        result = GetZoomInfo(is_zoom_in,&target_width,&target_height,anim_ctrl_ptr,&zoom_ratio);
        
        //may zoom
        if (result)
        {
            //get show width,height
            //获得图片显示的宽和高
            GUIANIM_GetShowWidthAndHeight(&shrect_width,
                &shrect_height,
                anim_ctrl_ptr);
            if (target_width < shrect_width)
            {
                shrect_width = target_width;
            }
            if (target_height < shrect_height)
            {
                shrect_height = target_height;
            }

            //get image clip rect after zoom
            cur_clip_rect = GetCurClipRect(shrect_width,
                                shrect_height,
                                target_width,
                                target_height,
                                anim_ctrl_ptr,
                                &zoom_ratio);

            //set src clip rect
            src_clip_rect.left   = (int16)(cur_clip_rect.left * anim_ctrl_ptr->img_width / target_width);
            src_clip_rect.top    = (int16)(cur_clip_rect.top * anim_ctrl_ptr->img_height / target_height);
            src_clip_rect.right  = (int16)(src_clip_rect.left + shrect_width * anim_ctrl_ptr->img_width / target_width - 1);
            src_clip_rect.bottom = (int16)(src_clip_rect.top + shrect_height * anim_ctrl_ptr->img_height / target_height - 1);

            //zoom image
            zoom_result = ZoomImage(shrect_width,shrect_height,&src_clip_rect,anim_ctrl_ptr);

            if (GUIANIM_RESULT_SUCC == zoom_result)
            {
                //get anim rect
                GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect);

                anim_ctrl_ptr->display_x = anim_rect.left;
                anim_ctrl_ptr->display_y = anim_rect.top;

                //adjust display position
                CTRLANIM_AdjustDisplayPosition(&anim_ctrl_ptr->display_x,
                    &anim_ctrl_ptr->display_y,
                    anim_ctrl_ptr->zoom_dest.buf_width,
                    anim_ctrl_ptr->zoom_dest.buf_height,
                    anim_rect,
                    anim_ctrl_ptr->display_info.align_style);

                //set adjust display pos flag
                anim_ctrl_ptr->is_adjust_disp_pos = TRUE;

                //set current image width and height
                anim_ctrl_ptr->zoom_dest.cur_img_width  = target_width;
                anim_ctrl_ptr->zoom_dest.cur_img_height = target_height;

                //set current image clip rect
                anim_ctrl_ptr->zoom_dest.cur_clip_rect = cur_clip_rect;

                if (is_update)
                {
                    //display
                    GUIANIM_Display(FALSE,FALSE,anim_ctrl_ptr);
                }

                result = TRUE;
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"GUIANIM_Zoom:zoom fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_ZOOM_652_112_2_18_3_15_6_50,(uint8*)"");
    }

    return (result);
}

/*****************************************************************************/
//  Description : get zoom info,include image width/height after zoom and ratio
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetZoomInfo(
                          BOOLEAN           is_zoom_in,         //in:
                          uint16            *target_width_ptr,  //in/out:
                          uint16            *target_height_ptr, //in/out:
                          CTRLANIM_OBJ_T    *anim_ctrl_ptr,     //in:
                          MMITHEME_RATIO_T  *ratio_ptr          //in/out:
                          )
{
    BOOLEAN             result = FALSE;
    uint16              width_max = 0;
    uint16              height_max = 0;
    uint16              target_width = 0;
    uint16              target_height = 0;
    MMITHEME_RATIO_T    zoom_ratio = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)anim_ctrl_ptr;

    //is need handle
    if (is_zoom_in)
    {
        //get zoom width range
        CTRLANIM_GetZoomWidthRange(base_ctrl_ptr->handle,PNULL,&width_max,PNULL,&height_max);

        //may zoom in
        if ((anim_ctrl_ptr->zoom_dest.cur_img_width < width_max) &&
            (anim_ctrl_ptr->theme.zoom_in_ratio.num1 > anim_ctrl_ptr->theme.zoom_in_ratio.num2))
        {
            //set zoom in ratio
            zoom_ratio = anim_ctrl_ptr->theme.zoom_in_ratio;

            //get target width and height after zoom in
            //获得放大后图片的宽高
            target_width  = (uint16)(anim_ctrl_ptr->zoom_dest.cur_img_width*zoom_ratio.num1/zoom_ratio.num2);
            target_height = (uint16)(anim_ctrl_ptr->zoom_dest.cur_img_height*zoom_ratio.num1/zoom_ratio.num2);

            //adjust target width and height
            if (target_width > width_max)
            {
                target_width  = width_max;
                target_height = height_max;

                //modify ratio
                zoom_ratio.num1 = target_width;
                zoom_ratio.num2 = anim_ctrl_ptr->zoom_dest.cur_img_width;
            }

            result = TRUE;
        }
    }
    else
    {
        //may zoom out
        if ((anim_ctrl_ptr->zoom_dest.cur_img_width > anim_ctrl_ptr->zoom_min_width) &&
            (anim_ctrl_ptr->theme.zoom_out_ratio.num1 < anim_ctrl_ptr->theme.zoom_out_ratio.num2))
        {
            //set zoom out ratio
            zoom_ratio = anim_ctrl_ptr->theme.zoom_out_ratio;

            //get target width and height after zoom out
            //获得缩小后图片的宽高
            target_width  = (uint16)(anim_ctrl_ptr->zoom_dest.cur_img_width*zoom_ratio.num1/zoom_ratio.num2);
            target_height = (uint16)(anim_ctrl_ptr->zoom_dest.cur_img_height*zoom_ratio.num1/zoom_ratio.num2);

            //adjust target width and height
            if (target_width < anim_ctrl_ptr->zoom_min_width)
            {
                target_width  = anim_ctrl_ptr->zoom_min_width;
                target_height = anim_ctrl_ptr->zoom_min_height;

                zoom_ratio.num1 = target_width;
                zoom_ratio.num2 = anim_ctrl_ptr->zoom_dest.cur_img_width;
            }

            result = TRUE;
        }
    }

    if (result)
    {
        //set output
        if (PNULL != ratio_ptr)
        {
            *ratio_ptr = zoom_ratio;
        }
        if (PNULL != target_width_ptr)
        {
            if (0 == target_width)
            {
                *target_width_ptr = 1;
            }
            else
            {
                *target_width_ptr = target_width;
            }
        }
        if (PNULL != target_height_ptr)
        {
            if (0 == target_height)
            {
                *target_height_ptr = 1;
            }
            else
            {
                *target_height_ptr = target_height;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get image clip rect after zoom
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetCurClipRect(
                                uint16              shrect_width,   //in:
                                uint16              shrect_height,  //in:
                                uint16              target_width,   //in:
                                uint16              target_height,  //in:
                                CTRLANIM_OBJ_T      *anim_ctrl_ptr, //in:
                                MMITHEME_RATIO_T    *zoom_ratio_ptr //in:
                                )
{
    GUI_RECT_T      cur_clip_rect = {0};
    GUI_POINT_T     center_point = {0};

    //get image display center point of rect
    //相对于缩放后图片的原点，获得其显示区域的中心点
    cur_clip_rect  = anim_ctrl_ptr->zoom_dest.cur_clip_rect;
    center_point.x = (int16)((cur_clip_rect.left + cur_clip_rect.right + 1)/2);
    center_point.y = (int16)((cur_clip_rect.top + cur_clip_rect.bottom + 1)/2);

    //get center point after zoom
    center_point.x = (int16)(center_point.x*zoom_ratio_ptr->num1/zoom_ratio_ptr->num2);
    center_point.y = (int16)(center_point.y*zoom_ratio_ptr->num1/zoom_ratio_ptr->num2);
    
    //set current image clip rect
    cur_clip_rect.left   = (int16)(center_point.x - shrect_width/2);
    cur_clip_rect.top    = (int16)(center_point.y - shrect_height/2);
    cur_clip_rect.right  = (int16)(center_point.x + shrect_width/2 - 1);
    cur_clip_rect.bottom = (int16)(center_point.y + shrect_height/2 - 1);

    //check current image clip rect
    if (0 > cur_clip_rect.left)
    {
        cur_clip_rect.right = (int16)(cur_clip_rect.right - cur_clip_rect.left);
        cur_clip_rect.left  = 0;
    }
    if (0 > cur_clip_rect.top)
    {
        cur_clip_rect.bottom = (int16)(cur_clip_rect.bottom - cur_clip_rect.top);
        cur_clip_rect.top    = 0;
    }
    if (cur_clip_rect.right >= target_width)
    {
        cur_clip_rect.left  = (int16)(cur_clip_rect.left + target_width - cur_clip_rect.right - 1);
        cur_clip_rect.right = (int16)(target_width - 1);
    }
    if (cur_clip_rect.bottom >= target_height)
    {
        cur_clip_rect.top    = (int16)(cur_clip_rect.top + target_height - cur_clip_rect.bottom - 1);
        cur_clip_rect.bottom = (int16)(target_height - 1);
    }

    return (cur_clip_rect);
}

/*****************************************************************************/
//  Description : handle anim zoom move
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_ZoomMove(
                                MMI_HANDLE_T    ctrl_handle,    //in:
                                int16           move_x,         //in:x<0 move left, x>0 move right
                                int16           move_y,         //in:y<0 move up, y>0 move bottom
                                BOOLEAN         is_update       //in:
                                )
{
    BOOLEAN             result = FALSE;
    uint16              shrect_width = 0;
    uint16              shrect_height = 0;
    GUI_RECT_T          cur_clip_rect = {0};
    GUI_RECT_T          src_clip_rect = {0};
    GUI_RECT_T          anim_rect = {0};
    CTRLANIM_OBJ_T      *anim_ctrl_ptr = PNULL;
    GUIANIM_RESULT_E    zoom_result = GUIANIM_RESULT_FAIL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if ((PNULL != anim_ctrl_ptr) &&
        (anim_ctrl_ptr->display_info.is_zoom) &&
        (anim_ctrl_ptr->is_support) &&
        (anim_ctrl_ptr->is_support_zoom) &&
        (anim_ctrl_ptr->is_decoded))
    {
        //set current image clip rect
        cur_clip_rect = anim_ctrl_ptr->zoom_dest.cur_clip_rect;

        //adjust move x
        if (0 > move_x)
        {
            //move left
            if (0 > (cur_clip_rect.left + move_x))
            {
                move_x = (int16)(-cur_clip_rect.left);
            }
        }
        else if (0 < move_x)
        {
            //move right
            if (anim_ctrl_ptr->zoom_dest.cur_img_width <= (cur_clip_rect.right + move_x))
            {
                move_x = (int16)(anim_ctrl_ptr->zoom_dest.cur_img_width - cur_clip_rect.right - 1);
            }
        }

        //adjust move y
        if (0 > move_y)
        {
            //move up
            if (0 > (cur_clip_rect.top + move_y))
            {
                move_y = (int16)(-cur_clip_rect.top);
            }
        }
        else if (0 < move_y)
        {
            //move bottom
            if (anim_ctrl_ptr->zoom_dest.cur_img_height <= (cur_clip_rect.bottom + move_y))
            {
                move_y = (int16)(anim_ctrl_ptr->zoom_dest.cur_img_height - cur_clip_rect.bottom - 1);
            }
        }

        if ((0 != move_x) || (0 != move_y))
        {
            //set current image clip rect after move
            cur_clip_rect.left   = (int16)(cur_clip_rect.left + move_x);
            cur_clip_rect.top    = (int16)(cur_clip_rect.top + move_y);
            cur_clip_rect.right  = (int16)(cur_clip_rect.right + move_x);
            cur_clip_rect.bottom = (int16)(cur_clip_rect.bottom + move_y);

            //get show width,height
            //获得图片显示的宽和高
            GUIANIM_GetShowWidthAndHeight(&shrect_width,
                &shrect_height,
                anim_ctrl_ptr);
            if (anim_ctrl_ptr->zoom_dest.cur_img_width < shrect_width)
            {
                shrect_width = anim_ctrl_ptr->zoom_dest.cur_img_width;
            }
            if (anim_ctrl_ptr->zoom_dest.cur_img_height < shrect_height)
            {
                shrect_height = anim_ctrl_ptr->zoom_dest.cur_img_height;
            }

            //set src clip rect
            src_clip_rect.left   = (int16)(cur_clip_rect.left * anim_ctrl_ptr->img_width / anim_ctrl_ptr->zoom_dest.cur_img_width);
            src_clip_rect.top    = (int16)(cur_clip_rect.top * anim_ctrl_ptr->img_width / anim_ctrl_ptr->zoom_dest.cur_img_width);
            src_clip_rect.right  = (int16)(src_clip_rect.left + shrect_width * anim_ctrl_ptr->img_width / anim_ctrl_ptr->zoom_dest.cur_img_width - 1);
            src_clip_rect.bottom = (int16)(src_clip_rect.top + shrect_height * anim_ctrl_ptr->img_height / anim_ctrl_ptr->zoom_dest.cur_img_height - 1);

            //zoom image
            zoom_result = ZoomImage(shrect_width,shrect_height,&src_clip_rect,anim_ctrl_ptr);

            if (GUIANIM_RESULT_SUCC == zoom_result)
            {
                //get anim rect
                GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect);

                anim_ctrl_ptr->display_x = anim_rect.left;
                anim_ctrl_ptr->display_y = anim_rect.top;

                //adjust display position
                CTRLANIM_AdjustDisplayPosition(&anim_ctrl_ptr->display_x,
                    &anim_ctrl_ptr->display_y,
                    anim_ctrl_ptr->zoom_dest.buf_width,
                    anim_ctrl_ptr->zoom_dest.buf_height,
                    anim_rect,
                    anim_ctrl_ptr->display_info.align_style);

                //set adjust display pos flag
                anim_ctrl_ptr->is_adjust_disp_pos = TRUE;

                //set current image clip rect
                anim_ctrl_ptr->zoom_dest.cur_clip_rect = cur_clip_rect;

                if (is_update)
                {
                    //display
                    GUIANIM_Display(FALSE,FALSE,anim_ctrl_ptr);
                }

                result = TRUE;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"GUIANIM_ZoomMove:move_x and move_y are zero!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_ZOOM_945_112_2_18_3_15_6_51,(uint8*)"");
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set anim zoom ratio
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetZoomRatio(
                                    MMI_HANDLE_T        ctrl_handle,        //in:
                                    MMITHEME_RATIO_T    *zoom_in_ratio_ptr, //in:may PNULL
                                    MMITHEME_RATIO_T    *zoom_out_ratio_ptr //in:may PNULL
                                    )
{
    BOOLEAN         result = FALSE;
    CTRLANIM_OBJ_T  *anim_ctrl_ptr = PNULL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if (PNULL != anim_ctrl_ptr)
    {
        if ((PNULL != zoom_in_ratio_ptr) &&
            (zoom_in_ratio_ptr->num1 > zoom_in_ratio_ptr->num2))
        {
            anim_ctrl_ptr->theme.zoom_in_ratio = *zoom_in_ratio_ptr;

            result = TRUE;
        }

        if ((PNULL != zoom_out_ratio_ptr) &&
            (zoom_out_ratio_ptr->num1 < zoom_out_ratio_ptr->num2))
        {
            anim_ctrl_ptr->theme.zoom_out_ratio = *zoom_out_ratio_ptr;

            result = TRUE;
        }
    }

    return (result);
}

#endif  //end of GUIF_ANIM_ZOOM
