#ifdef GUIF_ANIM_ROTATE
/*****************************************************************************
** File Name:      guianim_rotate.c                                          *
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
#include "graphics_draw.h"//xiyuan edit for pclint

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
//  Description : is rotate image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsRotate(
                       BOOLEAN              *is_mode_ptr,   //in/out:is rotate by mode
                       uint16               *angle_ptr,     //in/out:
                       uint16               img_width,      //in:
                       uint16               img_height,     //in:
                       GUIANIM_ROTATE_E     *mode_ptr       //in/out:
                       );

/*****************************************************************************/
//  Description : rotate image by mode or angle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RotateImage(
                          MMI_HANDLE_T      ctrl_handle,
                          uint16            angle,
                          GUIANIM_ROTATE_E  mode,
                          BOOLEAN           is_update
                          );

/*****************************************************************************/
//  Description : rotate image by rotate mode
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E RotateMode(
                                  CTRLANIM_OBJ_T    *anim_ctrl_ptr, //in
                                  GUIANIM_ROTATE_E  rotate_mode     //in:
                                  );

/*****************************************************************************/
//  Description : rotate image arbitrary angle
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E RotateArbitrary(
                                       CTRLANIM_OBJ_T   *anim_ctrl_ptr, //in
                                       uint16           angle           //in
                                       );

/*****************************************************************************/
//  Description : reset rotate image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E ResetRotate(
                                   CTRLANIM_OBJ_T   *anim_ctrl_ptr, //in
                                   uint16           angle           //in
                                   );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : is rotate image to horizontal display
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_IsRotateHorizontal(
                                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in
                                          )
{
    BOOLEAN     result = FALSE;

    if ((GUIANIM_ROTATE_90 == anim_ctrl_ptr->display_info.rotate_mode) ||
        (GUIANIM_ROTATE_270 == anim_ctrl_ptr->display_info.rotate_mode))
    {
        result = TRUE;
    }
    else
    {
        if ((90 == anim_ctrl_ptr->display_info.rotate_angle) ||
            (270 == anim_ctrl_ptr->display_info.rotate_angle))
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : auto rotate image to display in anim rect
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_AutoRotate(
                                  CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in
                                  )
{
    BOOLEAN             result = TRUE;
    BOOLEAN             is_rotate_mode = FALSE;
    GUIANIM_RESULT_E    rotate_result = GUIANIM_RESULT_FAIL;

    //is rotate
    anim_ctrl_ptr->is_rotate = IsRotate(&is_rotate_mode,
                                    &anim_ctrl_ptr->display_info.rotate_angle,
                                    anim_ctrl_ptr->img_width,
                                    anim_ctrl_ptr->img_height,
                                    &anim_ctrl_ptr->display_info.rotate_mode);
    //is rotate
    if (anim_ctrl_ptr->is_rotate)
    {
        if (is_rotate_mode)
        {
            //rotate image
            rotate_result = RotateMode(anim_ctrl_ptr,anim_ctrl_ptr->display_info.rotate_mode);
        }
        else
        {
            //rotate image
            rotate_result = RotateArbitrary(anim_ctrl_ptr,anim_ctrl_ptr->display_info.rotate_angle);
        }
        
        //set result
        if (GUIANIM_RESULT_SUCC == rotate_result)
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : is rotate image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsRotate(
                       BOOLEAN              *is_mode_ptr,   //in/out:is rotate by mode
                       uint16               *angle_ptr,     //in/out:
                       uint16               img_width,      //in:
                       uint16               img_height,     //in:
                       GUIANIM_ROTATE_E     *mode_ptr       //in/out:
                       )
{
    BOOLEAN     result = FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != angle_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != mode_ptr); /*assert verified*/
    if ((PNULL == angle_ptr)||(PNULL == mode_ptr))
    {
        return FALSE;
    }
    
    //rotate angle convert to rotate mode
    if (GUIANIM_ROTATE_NONE == *mode_ptr)
    {
        switch (*angle_ptr)
        {
        case 90:
            *mode_ptr = GUIANIM_ROTATE_90;
            break;

        case 180:
            *mode_ptr = GUIANIM_ROTATE_180;
            break;

        case 270:
            *mode_ptr = GUIANIM_ROTATE_270;
            break;

        default:
            break;
        }
    }

    //is rotate by rotate mode
    switch (*mode_ptr)
    {
    case GUIANIM_ROTATE_90:
    case GUIANIM_ROTATE_180:
    case GUIANIM_ROTATE_270:
    case GUIANIM_ROTATE_MIRROR:
        //src width and height必须2对齐
        if ((0 == (img_width % 2)) &&
            (0 == (img_height % 2)))
        {
            result = TRUE;
            if (PNULL != is_mode_ptr)
            {
                *is_mode_ptr = TRUE;
            }
        }
        break;

    default:
        SCI_TRACE_LOW("GUIANIM:IsRotate:rotate_mode %d is not rotate!",*mode_ptr);
        break;
    }

    //is rotate by rotate angle
    if (!result)
    {
        //rotate mode convert to rotate angle
        switch (*mode_ptr)
        {
        case GUIANIM_ROTATE_90:
            *angle_ptr = 90;
            break;

        case GUIANIM_ROTATE_180:
            *angle_ptr = 180;
            break;

        case GUIANIM_ROTATE_270:
            *angle_ptr = 270;
            break;

        case GUIANIM_ROTATE_MIRROR:
            *angle_ptr = 361;
            break;

        default:
            break;
        }

        if ((0 < *angle_ptr) && (360 > *angle_ptr))
        {
            result = TRUE;
        }
        else
        {
            SCI_TRACE_LOW("GUIANIM:IsRotate:angle %d is not rotate!",*angle_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : rotate image angle 90,180,270 or mirror
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_Rotate(
                              MMI_HANDLE_T      ctrl_handle,
                              GUIANIM_ROTATE_E  rotate_mode,
                              BOOLEAN           is_update
                              )
{
    BOOLEAN     result = FALSE;

    //rotate
    result = RotateImage(ctrl_handle,0,rotate_mode,is_update);

    return (result);
}

/*****************************************************************************/
//  Description : rotate image arbitrary angle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_RotateArbitrary(
                                       MMI_HANDLE_T     ctrl_handle,
                                       uint16           angle,
                                       BOOLEAN          is_update
                                       )
{
    BOOLEAN     result = FALSE;

    //rotate
    result = RotateImage(ctrl_handle,angle,GUIANIM_ROTATE_NONE,is_update);

    return (result);
}

/*****************************************************************************/
//  Description : rotate image by mode or angle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RotateImage(
                          MMI_HANDLE_T      ctrl_handle,
                          uint16            angle,
                          GUIANIM_ROTATE_E  mode,
                          BOOLEAN           is_update
                          )
{
    BOOLEAN             result = FALSE;
    BOOLEAN             is_rotate_mode = FALSE;
    uint16              rotate_angle = 0;
    GUI_RECT_T          anim_rect = {0};
    CTRLANIM_OBJ_T      *anim_ctrl_ptr = PNULL;
    GUIANIM_ROTATE_E    rotate_mode = GUIANIM_ROTATE_NONE;
    GUIANIM_RESULT_E    rotate_result = GUIANIM_RESULT_FAIL;

    //get anim control pointer
    anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
    if ((PNULL != anim_ctrl_ptr) &&
        (anim_ctrl_ptr->is_support) &&
        (anim_ctrl_ptr->is_decoded))
    {
        //set mode and angle
        rotate_mode  = mode;
        rotate_angle = angle;

        //is rotate
        anim_ctrl_ptr->is_rotate = IsRotate(&is_rotate_mode,
                                        &rotate_angle,
                                        anim_ctrl_ptr->img_width,
                                        anim_ctrl_ptr->img_height,
                                        &rotate_mode);

        if (anim_ctrl_ptr->is_rotate)
        {
            if (is_rotate_mode)
            {
                //rotate image
                rotate_result = RotateMode(anim_ctrl_ptr,rotate_mode);
            }
            else
            {
                //rotate image
                rotate_result = RotateArbitrary(anim_ctrl_ptr,rotate_angle);
            }
        }
        else
        {
            //reset rotate image
            rotate_result = ResetRotate(anim_ctrl_ptr,rotate_angle);
        }
        
        if (GUIANIM_RESULT_SUCC == rotate_result)
        {
            //get anim rect
            GUIANIM_GetAnimRect(anim_ctrl_ptr,&anim_rect);

            anim_ctrl_ptr->display_x = anim_rect.left;
            anim_ctrl_ptr->display_y = anim_rect.top;

            //adjust display position
            CTRLANIM_AdjustDisplayPosition(&anim_ctrl_ptr->display_x,
                &anim_ctrl_ptr->display_y,
                anim_ctrl_ptr->rotate_dest.img_width,
                anim_ctrl_ptr->rotate_dest.img_height,
                anim_rect,
                anim_ctrl_ptr->display_info.align_style);

            //set adjust display pos flag
            anim_ctrl_ptr->is_adjust_disp_pos = TRUE;

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
        SCI_TRACE_LOW("GUIANIM:RotateImage:rotate fail!");
    }

    return (result);
}

/*****************************************************************************/
//  Description : rotate image by rotate mode
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E RotateMode(
                                  CTRLANIM_OBJ_T    *anim_ctrl_ptr, //in
                                  GUIANIM_ROTATE_E  rotate_mode     //in:
                                  )
{
    GUIANIM_RESULT_E        result = GUIANIM_RESULT_FAIL;
    IMGREF_ROTATION_MODE_E  img_rotate = IMGREF_ROTATION_90;
    GUIANIM_ROTATE_INPUT_T  rotate_input = {0};
    GUIANIM_ROTATE_OUTPUT_T rotate_output = {0};

    //free rotate buffer
    if (PNULL != anim_ctrl_ptr->rotate_dest.buf_ptr)
    {
        SCI_FREE(anim_ctrl_ptr->rotate_dest.buf_ptr);
        anim_ctrl_ptr->rotate_dest.buf_size = 0;
        anim_ctrl_ptr->rotate_dest.buf_offset = 0;
    }

    //set rotate mode
    switch (rotate_mode)
    {
    case GUIANIM_ROTATE_90:
        img_rotate = IMGREF_ROTATION_90;
        break;

    case GUIANIM_ROTATE_180:
        img_rotate = IMGREF_ROTATION_180;
        break;

    case GUIANIM_ROTATE_270:
        img_rotate = IMGREF_ROTATION_270;
        break;

    case GUIANIM_ROTATE_MIRROR:
        img_rotate = IMGREF_ROTATION_MIRROR;
        break;

    default:
        break;
    }

    //set src
    rotate_input.src_buf_ptr    = anim_ctrl_ptr->target_buffer + anim_ctrl_ptr->target_buf_offset;
    rotate_input.src_buf_size   = anim_ctrl_ptr->target_buffer_size;
    rotate_input.src_img_width  = anim_ctrl_ptr->img_width;
    rotate_input.src_img_height = anim_ctrl_ptr->img_height;
    rotate_input.src_data_type  = anim_ctrl_ptr->target_data_type;
    rotate_input.rotate_mode    = img_rotate;

    //alloc rotate buffer,size 32bit对齐
    SCI_ASSERT(PNULL == anim_ctrl_ptr->rotate_dest.buf_ptr);/*assert verified*/
    if (GUIIMG_DATE_TYPE_ARGB888 == rotate_input.src_data_type)
    {
        anim_ctrl_ptr->rotate_dest.buf_size = (uint32)(anim_ctrl_ptr->img_width*anim_ctrl_ptr->img_height*4 + GUIANIM_BUF_ALIGN);
    }
    else
    {
        anim_ctrl_ptr->rotate_dest.buf_size = (uint32)(anim_ctrl_ptr->img_width*anim_ctrl_ptr->img_height*2 + GUIANIM_BUF_ALIGN);
    }
    anim_ctrl_ptr->rotate_dest.buf_size = (uint32)GUIANIM_ALIGN_EX(anim_ctrl_ptr->rotate_dest.buf_size,32);
    anim_ctrl_ptr->rotate_dest.buf_ptr  = SCI_ALLOCA(anim_ctrl_ptr->rotate_dest.buf_size);

    if (PNULL != anim_ctrl_ptr->rotate_dest.buf_ptr)
    {
        //reset buffer
        SCI_MEMSET(anim_ctrl_ptr->rotate_dest.buf_ptr,0,anim_ctrl_ptr->rotate_dest.buf_size);

        //set target buffer
        rotate_input.dest_buf_ptr  = anim_ctrl_ptr->rotate_dest.buf_ptr;
        rotate_input.dest_buf_size = anim_ctrl_ptr->rotate_dest.buf_size;

        //rotate image
        result = GUIANIM_RotateImage(&rotate_input,&rotate_output);
        if (GUIANIM_RESULT_SUCC == result)
        {
            //set rotate image width and height
            anim_ctrl_ptr->rotate_dest.img_width  = rotate_output.img_width;
            anim_ctrl_ptr->rotate_dest.img_height = rotate_output.img_height;

            //set rotate buffer offset
            anim_ctrl_ptr->rotate_dest.buf_offset = rotate_output.dest_buf_offset;
            
            //set rotate data type
            anim_ctrl_ptr->rotate_dest.data_type = rotate_output.data_type;
        }
    }
    else
    {
        SCI_TRACE_LOW("GUIANIM:RotateMode:no rotate buffer!");
    }

    if (GUIANIM_RESULT_SUCC != result)
    {
        anim_ctrl_ptr->is_rotate = FALSE;

        //free rotate buffer
        if (PNULL != anim_ctrl_ptr->rotate_dest.buf_ptr)
        {
            SCI_FREE(anim_ctrl_ptr->rotate_dest.buf_ptr);
            anim_ctrl_ptr->rotate_dest.buf_size = 0;
            anim_ctrl_ptr->rotate_dest.buf_offset = 0;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : rotate image arbitrary angle
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E RotateArbitrary(
                                       CTRLANIM_OBJ_T   *anim_ctrl_ptr, //in
                                       uint16           angle           //in
                                       )
{
    GUIANIM_RESULT_E                result = GUIANIM_RESULT_FAIL;
    GUIANIM_ROTATE_ANGLE_INPUT_T    rotate_input = {0};
    GUIANIM_ROTATE_ANGLE_OUTPUT_T   rotate_output = {0};
    IMGREF_SIZE_T                   src_size      = {0};
    IMGREF_SIZE_T                   des_size      = {0};

    //free rotate buffer
    if (PNULL != anim_ctrl_ptr->rotate_dest.buf_ptr)
    {
        SCI_FREE(anim_ctrl_ptr->rotate_dest.buf_ptr);
        anim_ctrl_ptr->rotate_dest.buf_size = 0;
        anim_ctrl_ptr->rotate_dest.buf_offset = 0;
    }

    //set src
    rotate_input.src_buf_ptr    = anim_ctrl_ptr->target_buffer + anim_ctrl_ptr->target_buf_offset;
    rotate_input.src_buf_size   = anim_ctrl_ptr->target_buffer_size;
    rotate_input.src_img_width  = anim_ctrl_ptr->img_width;
    rotate_input.src_img_height = anim_ctrl_ptr->img_height;
    rotate_input.src_data_type  = anim_ctrl_ptr->target_data_type;
    rotate_input.angle          = angle;    
    
    src_size.w = (uint16)anim_ctrl_ptr->img_width;
    src_size.h = (uint16)anim_ctrl_ptr->img_height;
    
    if (SCI_SUCCESS == GRAPH_GetRotatedSize(&src_size,angle,&des_size))
    {
        rotate_input.dest_img_width  = (uint16)des_size.w;
        rotate_input.dest_img_height = (uint16)des_size.h;
    }
    else
    {
        //set dest width and height
        rotate_input.dest_img_width  = (uint16)(anim_ctrl_ptr->img_width + anim_ctrl_ptr->img_height);
        rotate_input.dest_img_height = (uint16)(anim_ctrl_ptr->img_width + anim_ctrl_ptr->img_height);
    }
    

    //alloc rotate buffer,size 32bit对齐
    SCI_ASSERT(PNULL == anim_ctrl_ptr->rotate_dest.buf_ptr);/*assert verified*/
    anim_ctrl_ptr->rotate_dest.buf_size = (uint32)(rotate_input.dest_img_width*rotate_input.dest_img_height*4 + GUIANIM_BUF_ALIGN);
    anim_ctrl_ptr->rotate_dest.buf_size = (uint32)GUIANIM_ALIGN_EX(anim_ctrl_ptr->rotate_dest.buf_size,32);
    anim_ctrl_ptr->rotate_dest.buf_ptr  = SCI_ALLOCA(anim_ctrl_ptr->rotate_dest.buf_size);

    if (PNULL != anim_ctrl_ptr->rotate_dest.buf_ptr)
    {
        //reset buffer
        SCI_MEMSET(anim_ctrl_ptr->rotate_dest.buf_ptr,0,anim_ctrl_ptr->rotate_dest.buf_size);

        //set target buffer
        rotate_input.dest_buf_ptr  = anim_ctrl_ptr->rotate_dest.buf_ptr;
        rotate_input.dest_buf_size = anim_ctrl_ptr->rotate_dest.buf_size;

        //rotate image
        result = GUIANIM_RotateAngle(&rotate_input,&rotate_output);
        if (GUIANIM_RESULT_SUCC == result)
        {
            //set rotate image width and height
            anim_ctrl_ptr->rotate_dest.img_width  = rotate_output.img_width;
            anim_ctrl_ptr->rotate_dest.img_height = rotate_output.img_height;

            //set rotate buffer offset
            anim_ctrl_ptr->rotate_dest.buf_offset = 0;

            //set rotate data type
            anim_ctrl_ptr->rotate_dest.data_type = rotate_output.data_type;
        }
    }
    else
    {
        SCI_TRACE_LOW("GUIANIM:RotateArbitrary:no rotate buffer!");
    }

    if (GUIANIM_RESULT_SUCC != result)
    {
        anim_ctrl_ptr->is_rotate = FALSE;

        //free rotate buffer
        if (PNULL != anim_ctrl_ptr->rotate_dest.buf_ptr)
        {
            SCI_FREE(anim_ctrl_ptr->rotate_dest.buf_ptr);
            anim_ctrl_ptr->rotate_dest.buf_size = 0;
            anim_ctrl_ptr->rotate_dest.buf_offset = 0;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : reset rotate image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E ResetRotate(
                                   CTRLANIM_OBJ_T   *anim_ctrl_ptr, //in
                                   uint16           angle           //in
                                   )
{
    GUIANIM_RESULT_E                result = GUIANIM_RESULT_FAIL;

    if ((0 == angle) || (360 == angle))
    {
        //free rotate buffer
        if (PNULL != anim_ctrl_ptr->rotate_dest.buf_ptr)
        {
            SCI_FREE(anim_ctrl_ptr->rotate_dest.buf_ptr);
            anim_ctrl_ptr->rotate_dest.buf_size = 0;
            anim_ctrl_ptr->rotate_dest.buf_offset = 0;
        }

        //set rotate image width and height
        anim_ctrl_ptr->rotate_dest.img_width  = anim_ctrl_ptr->img_width;
        anim_ctrl_ptr->rotate_dest.img_height = anim_ctrl_ptr->img_height;

        result = GUIANIM_RESULT_SUCC;
    }

    return (result);
}

#endif  //end of GUIF_ANIM_ROTATE